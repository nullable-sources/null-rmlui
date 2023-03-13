#pragma once
#include <RmlUi/Core/ElementInstancer.h>
#include <RmlUi/Core/EventListenerInstancer.h>

#include <sol/sol.hpp>

namespace null::rml::modules::lua {
	class c_document_element_instancer : public Rml::ElementInstancer {
	protected:
		sol::state_view state_view{ nullptr };

	public:
		c_document_element_instancer(const sol::state_view& _state_view) : state_view{ _state_view } { }
		
	public:
		Rml::ElementPtr InstanceElement(Rml::Element* parent, const std::string& tag, const Rml::XMLAttributes& attributes) override;
		void ReleaseElement(Rml::Element* element) override;
	};

	class c_event_listener_instancer : public Rml::EventListenerInstancer {
	protected:
		sol::state_view state_view{ nullptr };

	public:
		c_event_listener_instancer(const sol::state_view& _state_view) : state_view{ _state_view } { }

	public:
		Rml::EventListener* InstanceEventListener(const std::string& value, Rml::Element* element) override;
	};

}
