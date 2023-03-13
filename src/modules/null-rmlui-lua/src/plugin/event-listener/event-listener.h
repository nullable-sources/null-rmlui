#pragma once

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/EventListener.h>
#include <sol/sol.hpp>

namespace Rml {
    class Element;
    class ElementDocument;
}

namespace null::rml::modules::lua {
    class c_event_listener : public Rml::EventListener {
    private:
        sol::protected_function func{ };
        Rml::Element* element{ };

    public:
        c_event_listener(sol::protected_function _func, Rml::Element* _element) : func{ _func }, element{ _element } { }
        c_event_listener(sol::state_view& state_view, const std::string& code, Rml::Element* _element);

    public:
        void OnDetach(Rml::Element* element) override { delete this; }
        void ProcessEvent(Rml::Event& event) override;
    };
}
