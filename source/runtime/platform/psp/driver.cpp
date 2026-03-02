#include <string>
#include <optional>

extern "C" {
#include <pspexploit.h>
}

#include <pspusb.h>
#include <pspusbbus.h>
#include <pspusbstor.h>
#include <pspdebug.h>
#include <pspmodulemgr.h>  

#include <platform/psp/driver.hpp>

#define NID_sceKernelLoadModule 0xFFB9B760
#define NID_sceKernelLoadModuleBuffer 0x207EDD42
#define NID_sceKernelLoadModuleMs 0x710F61B5
#define NID_ssceKernelLoadModuleForLoadExecForUser 0x32292450
#define NID_sceKernelStartModule 0xE6BF3960
#define NID_sceUsbStart 0xAE5DE6AF
#define NID_sceUsbStop 0xC2464FA0
#define NID_sceUsbActivate 0x586DB82C
#define NID_sceUsbbdRegister 0xB1644BE7

#define USB_DT_DEVICE        0x01
#define USB_DT_CONFIG        0x02
#define USB_DT_STRING        0x03
#define USB_DT_INTERFACE     0x04
#define USB_DT_ENDPOINT      0x05
#define USB_DT_HID           0x21
#define USB_DT_REPORT        0x22

#define USB_REQ_GET_DESCRIPTOR 0x06

#define HID_REQ_GET_PROTOCOL  0x03
#define HID_REQ_SET_PROTOCOL  0x0B
#define HID_REQ_SET_IDLE      0x0A
#define HID_REQ_GET_IDLE      0x02



namespace {

static const u8 g_hid_report_desc[] = {
		0x06, 0x00, 0xFF,        // Usage Page (Vendor Defined 0xFF00)
		0x09, 0x01,              // Usage (0x01)
		0xA1, 0x01,              // Collection (Application)
		0x85, 0x01,              //   Report ID (1)

		// OUT report: 64 bytes
		0x09, 0x02,              //   Usage (0x02)
		0x15, 0x00,              //   Logical Min (0)
		0x26, 0xFF, 0x00,        //   Logical Max (255)
		0x75, 0x08,              //   Report Size (8)
		0x95, 0x40,              //   Report Count (64)
		0x91, 0x02,              //   Output (Data,Var,Abs)

		// IN report: 64 bytes
		0x09, 0x03,              //   Usage (0x03)
		0x15, 0x00,              //   Logical Min (0)
		0x26, 0xFF, 0x00,        //   Logical Max (255)
		0x75, 0x08,              //   Report Size (8)
		0x95, 0x40,              //   Report Count (64)
		0x81, 0x02,              //   Input (Data,Var,Abs)

		0xC0                     // End Collection
};

static const u8 g_hid_conf_blob[] = {
	// Interface Descriptor
	0x09, USB_DT_INTERFACE,
	0x01,       // bInterfaceNumber (use 1 to avoid MSC (usually 0))
	0x00,       // bAlternateSetting
	0x02,       // bNumEndpoints
	0x03, 0x00, 0x00, // bInterfaceClass=HID, SubClass=0, Protocol=0
	0x00,       // iInterface

	// HID Descriptor
	0x09, USB_DT_HID,
	0x11, 0x01, // bcdHID 1.11
	0x00,       // bCountryCode
	0x01,       // bNumDescriptors
	USB_DT_REPORT, (u8)(sizeof(g_hid_report_desc) & 0xFF), (u8)(sizeof(g_hid_report_desc) >> 8),

	// Endpoint Descriptor (Interrupt IN) EP 0x81
	0x07, USB_DT_ENDPOINT,
	0x81,       // bEndpointAddress
	0x03,       // bmAttributes = Interrupt
	0x40, 0x00, // wMaxPacketSize = 64
	0x01,       // bInterval = 1ms (FS)

	// Endpoint Descriptor (Interrupt OUT) EP 0x01
	0x07, USB_DT_ENDPOINT,
	0x01,       // bEndpointAddress
	0x03,       // bmAttributes = Interrupt
	0x40, 0x00, // wMaxPacketSize = 64
	0x01        // bInterval
};

static UsbEndpoint g_endp[] = {
	{ 0x00, 0, 0 }, // EP0
	{ 0x81, 0, 0 }, // IN
	{ 0x01, 0, 0 }, // OUT
};

static UsbInterface g_intf[] = {
	{ 1, 0, 1 }     // expect_interface=1, num_interface=1
};

/* Strings can be NULL; keep it NULL for minimal setup */
static int g_hid_protocol = 1;
static int g_hid_idle = 0;

/* recvctl convention commonly used on PSP:
   - arg2 is treated as a pointer to the control transfer data buffer.
   - return value is number of bytes provided (for IN data stage), or 0 for status-only.
*/
static int hid_recvctl(int arg1, int arg2, DeviceRequest* req)
{
	(void)arg1;
	u8* buf = (u8*)arg2;

	const u8 reqType = req->bmRequestType;
	const u8 bReq = req->bRequest;

	// Standard GET_DESCRIPTOR
	if (bReq == USB_REQ_GET_DESCRIPTOR && (reqType & 0x80)) {
		const u8 dtype = (u8)(req->wValue >> 8);
		// const u8 dindex = (u8)(req->wValue & 0xFF);

		if (dtype == USB_DT_REPORT) {
			int n = (int)sizeof(g_hid_report_desc);
			if (n > req->wLength) n = req->wLength;
			memcpy(buf, g_hid_report_desc, n);
			return n;
		}
		if (dtype == USB_DT_HID) {
			// HID descriptor is the 9 bytes starting at offset 9 in g_hid_conf_blob
			const u8* hid = &g_hid_conf_blob[9];
			int n = 9;
			if (n > req->wLength) n = req->wLength;
			memcpy(buf, hid, n);
			return n;
		}
	}

	// HID class requests (interface recipient)
	if ((reqType & 0x60) == 0x20) { // Class
		switch (bReq) {
		case HID_REQ_GET_PROTOCOL:
			if (reqType & 0x80) { buf[0] = (u8)g_hid_protocol; return (req->wLength >= 1) ? 1 : 0; }
			return 0;
		case HID_REQ_SET_PROTOCOL:
			g_hid_protocol = (req->wValue & 0xFF);
			return 0;
		case HID_REQ_GET_IDLE:
			if (reqType & 0x80) { buf[0] = (u8)g_hid_idle; return (req->wLength >= 1) ? 1 : 0; }
			return 0;
		case HID_REQ_SET_IDLE:
			g_hid_idle = (req->wValue >> 8) & 0xFF;
			return 0;
		default:
			return 0;
		}
	}

	return 0;
}

static int hid_attach(int speed, void* a2, void* a3) { (void)speed; (void)a2; (void)a3; return 0; }
static int hid_detach(int a, int b, int c) { (void)a; (void)b; (void)c; return 0; }
static int hid_start(int size, void* args) { (void)size; (void)args; return 0; }
static int hid_stop(int size, void* args) { (void)size; (void)args; return 0; }

static UsbDriver g_hid_driver = {
	.name = "HIDVendorMsg",
	.endpoints = 3,
	.endp = g_endp,
	.intp = g_intf,
	.devp_hi = NULL,
	.confp_hi = NULL,
	.devp = NULL,
	.confp = (void*)g_hid_conf_blob,
	.str = NULL,
	.recvctl = hid_recvctl,
	.func28 = NULL,
	.attach = hid_attach,
	.detach = hid_detach,
	.unk34 = 0,
	.start_func = hid_start,
	.stop_func = hid_stop,
	.link = NULL
};

	using _sceKernelLoadModuleFunction = SceUID(*)(const char* path, int flags, SceKernelLMOption* options);
	using _sceKernelStartModuleFunction = int (*)(SceUID module_id, SceSize argc, void* argv, int* status, SceKernelLMOption* options);
	using _sceUsbStartFunction = int (*)(const char* driver_name, int size, void* args);
	using _sceUsbStopFunction = int (*)(const char* driver_name, int size, void* args);
	using _sceUsbActivateFunction = int (*)(u32 pid);
	using _sceUsbbdRegisterFunction = int (*)(UsbDriver* driver);

	static _sceKernelLoadModuleFunction _sceKernelLoadModule = nullptr;
	static _sceKernelStartModuleFunction _sceKernelStartModule = nullptr;
	static _sceUsbStartFunction _sceUsbStart = nullptr;
	static _sceUsbStopFunction _sceUsbStop = nullptr;
	static _sceUsbActivateFunction _sceUsbActivate = nullptr;
	static _sceUsbbdRegisterFunction _sceUsbbdRegister = nullptr;

	static KernelFunctions _kernel_functions;

	void _load_and_start_module(const char* path)
	{
		const SceUID _module_id = _sceKernelLoadModule(path, 0, NULL);
		if (_module_id < 0) {
			pspDebugScreenPrintf("Failed to load %s module\n", path);
			return;
		}

		int status;
		if (_sceKernelStartModule(_module_id, 0, NULL, &status, NULL) < 0) {
			pspDebugScreenPrintf("Failed to start %s module\n", path);
		}

		pspDebugScreenPrintf("Loaded and started module %s\n", path);
	}

	template <typename FunctionPtr>
	bool _ensure_function_found(FunctionPtr* function_ptr, const std::string& module_name, const std::string& library_name, const std::size_t nid)
	{
		u32 _found_address = pspXploitFindFunction(module_name.c_str(), library_name.c_str(), nid);
		if (!_found_address) {
			pspDebugScreenPrintf("Failed to load function from %s at 0x%08X\n", library_name.c_str(), _found_address);
			return false;
		}
		*function_ptr = reinterpret_cast<FunctionPtr>(_found_address);
		pspDebugScreenPrintf("Found kernel function from %s at 0x%08X\n", library_name.c_str(), _found_address);
		return true;
	}

	void _kernel_scan_callback()
	{
		const int _memory_k1 = pspSdkSetK1(0);
		const int _memory_level = pspXploitSetUserLevel(8);
		pspXploitScanKernelFunctions(&_kernel_functions);

		_ensure_function_found(&_sceKernelLoadModule, "sceModuleManager", "ModuleMgrForKernel", NID_sceKernelLoadModule);
		_ensure_function_found(&_sceKernelStartModule, "sceModuleManager", "ModuleMgrForKernel", NID_sceKernelStartModule);
		_ensure_function_found(&_sceUsbStart, "sceUSB_Driver", "sceUsb", NID_sceUsbStart);
		_ensure_function_found(&_sceUsbStop, "sceUSB_Driver", "sceUsb", NID_sceUsbStop);
		_ensure_function_found(&_sceUsbActivate, "sceUSB_Driver", "sceUsb", NID_sceUsbActivate);
		_ensure_function_found(&_sceUsbbdRegister, "sceUSB_Driver", "sceUsbBus_driver", NID_sceUsbbdRegister);

		_load_and_start_module("flash0:/kd/semawm.prx");
		_load_and_start_module("flash0:/kd/usbstor.prx");
		_load_and_start_module("flash0:/kd/usbstormgr.prx");
		_load_and_start_module("flash0:/kd/usbstorms.prx");

		int r = _sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
		pspDebugScreenPrintf("Tried USB Bus driver registration with result 0x%08X\n", r);

		int u = _sceUsbbdRegister(&g_hid_driver);
		pspDebugScreenPrintf("Tried USB Composite driver registration with result 0x%08X\n", u);

		int t = _sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
		pspDebugScreenPrintf("Tried USB Mass Storage registration with result 0x%08X\n", t);

		int s = _sceUsbActivate(0x1C00);
		pspDebugScreenPrintf("Tried USB activation with result0x%08X\n", s);

		_kernel_functions.KernelDelayThread(4e8);

		pspXploitRepairKernel();
		pspXploitSetUserLevel(_memory_level);
		pspSdkSetK1(_memory_k1);
	}

}

usb_driver::usb_driver()
{
	static std::optional<bool> _is_valid_memory = false;
	
	if (_is_valid_memory) {
		_is_valid = _is_valid_memory.value();
		return;
	}

	if (pspXploitInitKernelExploit() < 0) {
		_is_valid = false;
		return;
	}

	if (pspXploitDoKernelExploit() < 0) {
		_is_valid = false;
		return;
	}

	pspXploitExecuteKernel((u32)_kernel_scan_callback);
	_is_valid_memory = true;
	_is_valid = true;
}

bool usb_driver::is_valid() const
{
	return _is_valid;
}
