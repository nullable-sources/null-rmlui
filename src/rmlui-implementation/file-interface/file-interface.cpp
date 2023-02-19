#include <file-interface/file-interface.h>

namespace null::rml {
	Rml::FileHandle c_file_interface::Open(const Rml::String& path) {
		std::string new_path{ path };
		if(std::ranges::any_of(file_loaders, [&](const auto& loader) { return loader->create(new_path); }))
			return (Rml::FileHandle)i_file_loader::opened_files[new_path].get();
		
		throw std::runtime_error{ std::format("couldn't find a suitable loader for '{}'", path) };
		return Rml::FileHandle{ };
	}
}