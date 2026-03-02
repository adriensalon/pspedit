#pragma once

namespace pspedit {
	
struct usb_driver {
	usb_driver();
	usb_driver(const usb_driver& other) = delete;
	usb_driver& operator=(const usb_driver& other) = delete;
	usb_driver(usb_driver&& other) = delete;
	usb_driver& operator=(usb_driver&& other) = delete;	

	[[nodiscard]] bool is_valid() const;

private:
	bool _is_valid;
};

}