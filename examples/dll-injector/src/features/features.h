#pragma once
#include <string>
#include <vector>

namespace features {
	struct process_info_t {
	public:
		std::string name{ };
		std::uint32_t pid{ };
		std::string type{ };
		std::string architecture{ };
	};

	struct dll_info_t {
	public:
		std::string name{ };
		std::string path{ };
		std::string architecture{ };

	public:
		dll_info_t() { }
		dll_info_t(const std::string_view& _path) : path{ _path }, name{ _path.substr(_path.find_last_of('\\') + 1) } { }
	};

	void inject(const std::uint32_t& pid, const std::string& dll_path);
	std::vector<process_info_t> get_processes_list();
	dll_info_t select_dll();
}