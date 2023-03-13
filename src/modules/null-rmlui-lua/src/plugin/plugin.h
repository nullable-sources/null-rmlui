#pragma once
#include <memory>

#include <RmlUi/Lua/Header.h>
#include <RmlUi/Core/Plugin.h>
#include <RmlUi/Core/Platform.h>

#include <null-rmlui-lua.h>

#include <plugin/instancer/instancer.h>

namespace null::rml::modules::lua {
    class c_document_element_instancer;

    class c_plugin : public Rml::Plugin {
    private:
        std::unique_ptr<c_document_element_instancer> document_element_instancer{ };
        std::unique_ptr<c_event_listener_instancer> event_listener_instancer{ };
        
        sol::state_view state_view{ nullptr };

    public:
        c_plugin(const sol::state_view& _state_view) : state_view{ _state_view } { }

    private:
        int GetEventClasses() override { return EVT_BASIC; }

        void OnInitialise() override;
        void OnShutdown() override;
    };
}
