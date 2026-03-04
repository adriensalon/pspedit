#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace pspedit {

	struct messenger_driver {
		messenger_driver(const std::filesystem::path& inbound_dir, const std::filesystem::path& outbound_dir) noexcept;
		messenger_driver(const messenger_driver& other) = delete;
		messenger_driver& operator=(const messenger_driver& other) = delete;
		messenger_driver(messenger_driver&& other) noexcept = default;
		messenger_driver& operator=(messenger_driver&& other) noexcept = default;
		~messenger_driver() noexcept;

		bool is_valid() const noexcept;
		bool poll_messages(std::vector<std::string>& messages) noexcept;
		bool send_message(const std::string& message) noexcept;
	
	private:
		bool _is_valid;
		std::filesystem::path _inbound_dir;
		std::filesystem::path _outbound_dir;
		std::uint64_t _seq_id;
	};

}