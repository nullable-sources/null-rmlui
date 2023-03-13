#pragma once
#include <RmlUi/Core.h>
#include <null-lua.h>

namespace null::rml::modules::lua {
    void initialize(sol::state_view& state_view);
    void register_bindings(sol::state_view& state_view);
}