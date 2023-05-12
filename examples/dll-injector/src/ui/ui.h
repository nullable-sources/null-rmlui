#pragma once
#include <null-rmlui-renderer-directx11.h>
#include <null-render-backend-directx11.h>
#include <null-rmlui.h>

namespace ui {
	inline null::render::backend::directx11::c_window window{ };

	inline Rml::Context* context{ };
	inline Rml::ElementDocument* document{ };

	class c_event_listener : public Rml::EventListener {
	public:
		void ProcessEvent(Rml::Event& event) override;
	} inline event_listener{ };

	void initialize();
}