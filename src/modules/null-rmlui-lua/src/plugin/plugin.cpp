#include "plugin/plugin.h"

#include "bindings/bindings.h"

namespace null::rml::modules::lua {
	void c_plugin::OnInitialise() {
		document_element_instancer = std::make_unique<c_document_element_instancer>(state_view);
		event_listener_instancer = std::make_unique<c_event_listener_instancer>(state_view);
		Rml::Factory::RegisterElementInstancer("body", document_element_instancer.get());
		Rml::Factory::RegisterEventListenerInstancer(event_listener_instancer.get());
	}

	void c_plugin::OnShutdown() {
		state_view.collect_garbage();
		delete this;
	}
}
