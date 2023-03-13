#pragma once
#include <null-lua.h>

namespace null::rml::modules::lua::event_bindings {
	void bind(sol::state_view & state_view);
}