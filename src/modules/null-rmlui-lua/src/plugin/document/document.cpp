#include <format>
#include <RmlUi/Core/Stream.h>
#include <RmlUi/Core/Log.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/FileInterface.h>

#include "plugin/document/document.h"

namespace null::rml::modules::lua{
	sol::protected_function_result error_handler(lua_State*, sol::protected_function_result pfr) {
		if(!pfr.valid()) {
			Rml::Log::Message(Rml::Log::LT_ERROR, "[lua] %s", pfr.get<sol::error>().what());
		}
		return pfr;
	}

	void c_document::LoadInlineScript(const std::string& content, const std::string& source_path, int source_line) {
		state_view.safe_script(std::format("--[{}][{}]:[{}]\n{}", GetContext()->GetName(), GetSourceURL(), source_line, content), environment, error_handler);
	}

	void c_document::LoadExternalScript(const std::string& source_path) {
		Rml::FileInterface* file_interface{ Rml::GetFileInterface() };
		Rml::FileHandle handle{ file_interface->Open(source_path) };
		if(handle == 0) {
			Rml::Log::Message(Rml::Log::LT_WARNING, "LoadFile: Unable to open file: %s", source_path.c_str());
			return;
		}

		size_t size{ file_interface->Length(handle) };
		if(size == 0) {
			Rml::Log::Message(Rml::Log::LT_WARNING, "LoadFile: File is 0 bytes in size: %s", source_path.c_str());
			return;
		}
		std::unique_ptr<char[]> file_contents{ new char[size] };
		file_interface->Read(file_contents.get(), size, handle);
		file_interface->Close(handle);

		run_script(std::string{ file_contents.get(), size });
	}
}
