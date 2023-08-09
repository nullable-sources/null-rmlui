#include "bindings/bindings.h"

namespace null::rml::modules::lua::log_bindings {
	void bind(sol::state_view& state_view) {
		sol::table rmlui{ state_view["rmlui"].get<sol::table>() };
		rmlui.new_enum<Rml::Log::Type>("e_log_type", {
				{ "always", Rml::Log::LT_ALWAYS },
				{ "error", Rml::Log::LT_ERROR },
				{ "warning", Rml::Log::LT_WARNING },
				{ "info", Rml::Log::LT_INFO },
				{ "debug", Rml::Log::LT_DEBUG }
			}
		);

		rmlui.create_named("log").set_function("message", [](Rml::Log::Type type, const std::string& message) { Rml::Log::Message(type, "%s", message.c_str()); });
	}
}