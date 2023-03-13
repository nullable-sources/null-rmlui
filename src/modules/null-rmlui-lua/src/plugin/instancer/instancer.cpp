#include <plugin/instancer/instancer.h>

#include <plugin/document/document.h>
#include <plugin/event-listener/event-listener.h>


namespace null::rml::modules::lua {
	Rml::ElementPtr c_document_element_instancer::InstanceElement(Rml::Element* parent, const std::string& tag, const Rml::XMLAttributes& attributes) {
		return Rml::ElementPtr{ new c_document{ state_view, tag } };
	}

	void c_document_element_instancer::ReleaseElement(Rml::Element* element) {
		delete element;
	}

	Rml::EventListener* c_event_listener_instancer::InstanceEventListener(const std::string& value, Rml::Element* element) {
		return new c_event_listener{ state_view, value, element };
	}
}
