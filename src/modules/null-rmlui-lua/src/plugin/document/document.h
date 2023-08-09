#pragma once
#include <RmlUi/Core/ElementDocument.h>

#include <sol/sol.hpp>

namespace null::rml::modules::lua {
	sol::protected_function_result error_handler(lua_State*, sol::protected_function_result pfr);

	class c_document : public Rml::ElementDocument {
	public:
		sol::state_view state_view{ nullptr };
		sol::environment environment{ };

	public:
		c_document(const sol::state_view& _state_view, const std::string& tag)
			: state_view{ _state_view }, ElementDocument{ tag }, environment{ state_view, sol::create, state_view.globals() } { }

	public:
		void LoadInlineScript(const std::string& content, const std::string& source_path, int source_line) override;

		void LoadExternalScript(const std::string& source_path) override;

	public:
		sol::protected_function_result run_script(const std::string& script) {
			return state_view.safe_script(script, environment, error_handler);
		}
	};
}