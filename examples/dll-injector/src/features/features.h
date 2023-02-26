#pragma once
#include <string>
#include <vector>

namespace features {
	struct process_info_t {
	public:
		std::string name{ };
		std::uint32_t pid{ };
	};

	void inject(const std::uint32_t& pid, const std::string& dll_path);
	std::vector<process_info_t> get_processes_list();
	std::string select_dll();
}