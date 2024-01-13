#include "file-interface.h"

namespace null::rml {
	Rml::FileHandle c_file_interface::Open(const Rml::String& path) {
		std::string new_path = path;
		if(std::ranges::any_of(file_loaders, std::bind(std::mem_fn(&i_file_loader::create), std::placeholders::_1, std::ref(new_path))))
			return (Rml::FileHandle)i_file_loader::opened_files[new_path].get();
		
		utils::logger(utils::e_log_type::warning, "couldn't find a suitable loader for '{}'", path);
		return Rml::FileHandle{ };
	}
}