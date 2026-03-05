#include <algorithm>
#include <chrono>
#include <fstream>
#include <memory>
#include <random>
#include <sstream>

#include <core/messenger.hpp>

namespace pspedit {
namespace {

    [[nodiscard]] static bool _ensure_layout(const std::filesystem::path& base_dir)
    {
        return (std::filesystem::create_directories(base_dir / "inbox")
            && std::filesystem::create_directories(base_dir / "temp"));
    }

    [[nodiscard]] static std::uint32_t _random_u32()
    {
        static thread_local std::mt19937 _rng { std::random_device {}() };
        return _rng();
    }

    [[nodiscard]] static std::uint64_t _unix_ms()
    {
        return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    }

    [[nodiscard]] static std::string _make_name(std::uint64_t& seq_id)
    {
        std::ostringstream oss;
        oss << _unix_ms() << "_"
            << std::setw(6) << std::setfill('0') << (seq_id++) << "_"
            << std::hex << std::setw(8) << std::setfill('0') << _random_u32();
        return oss.str();
    }

    [[nodiscard]] bool _compare_dirs(const std::filesystem::directory_entry& first_dir, const std::filesystem::directory_entry& second_dir)
    {
        return first_dir.path().filename().string() < second_dir.path().filename().string();
    }

}

messenger_driver::messenger_driver(const std::filesystem::path& inbound_dir, const std::filesystem::path& outbound_dir) noexcept
    : _is_valid(_ensure_layout(inbound_dir) && _ensure_layout(outbound_dir))
    , _inbound_dir(inbound_dir)
    , _outbound_dir(outbound_dir)
    , _seq_id(0)
{
}

messenger_driver::~messenger_driver() noexcept
{
    // TODO remove folders ?
}

bool messenger_driver::is_valid() const noexcept
{
    return _is_valid;
}

bool messenger_driver::poll_messages(std::vector<std::string>& messages) noexcept
{
    if (!_is_valid) {
        return false;
    }

    std::vector<std::filesystem::directory_entry> _inbox_entries;
    const std::filesystem::path _inbox_dir = _inbound_dir / "inbox";
    for (const std::filesystem::directory_entry& _inbox_entry : std::filesystem::directory_iterator(_inbox_dir)) {
        if (!_inbox_entry.is_regular_file()) {
            continue;
        }
        if (_inbox_entry.path().extension() == ".msg") {
            _inbox_entries.push_back(_inbox_entry);
        }
    }
    std::sort(_inbox_entries.begin(), _inbox_entries.end(), _compare_dirs);

    messages.reserve(messages.size() + _inbox_entries.size());
    for (std::filesystem::directory_entry& _inbox_entry : _inbox_entries) {
        const std::filesystem::path _inbox_entry_path = _inbox_entry.path();

        std::string _message_data;
        std::unique_ptr<std::ifstream> _fstream_ptr = std::make_unique<std::ifstream>(_inbox_entry_path, std::ios::binary);
        if (_fstream_ptr->fail()) {
            continue;
        }
        _fstream_ptr->seekg(0, std::ios::end);
        std::streampos sz = _fstream_ptr->tellg();
        if (sz < 0) {
            continue;
        }
        _message_data.resize(static_cast<std::size_t>(sz));
        _fstream_ptr->seekg(0, std::ios::beg);
        _fstream_ptr->read(&_message_data[0], sz);
        if (_fstream_ptr->fail() && sz != 0) {
            continue;
        }
        _fstream_ptr.reset();

        messages.push_back(std::move(_message_data));
        if (!std::filesystem::remove(_inbox_entry_path)) {
            return false;
        }
    }

    return true;
}

bool messenger_driver::send_message(const std::string& message) noexcept
{
    if (!_is_valid) {
        return false;
    }

    const std::string _message_name = _make_name(_seq_id);
    const std::filesystem::path _message_temp_path = _outbound_dir / "temp" / (_message_name + ".tmp");

    std::unique_ptr<std::ofstream> _fstream_ptr = std::make_unique<std::ofstream>(_message_temp_path, std::ios::binary | std::ios::trunc);
    if (_fstream_ptr->fail()) {
        return false;
    }
    _fstream_ptr->write(message.data(), static_cast<std::streamsize>(message.size()));
    _fstream_ptr->flush();
    if (_fstream_ptr->fail()) {
        return false;
    }
    _fstream_ptr.reset();

    const std::filesystem::path _message_inbox_path = _outbound_dir / "inbox" / (_message_name + ".msg");
    std::filesystem::rename(_message_temp_path, _message_inbox_path);
    return true;
}

}