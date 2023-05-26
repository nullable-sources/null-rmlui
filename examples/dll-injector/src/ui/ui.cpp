#include "ui.h"

namespace ui {
	void c_event_listener::ProcessEvent(Rml::Event& event) {
		static POINT offset{ };
		switch(event.GetId()) {
			case Rml::EventId::Handledrag: {
				POINT cursor_pos{ };
				GetCursorPos(&cursor_pos);

				SetWindowPos(window.wnd_handle, nullptr, cursor_pos.x - offset.x, cursor_pos.y - offset.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			} break;

			case Rml::EventId::Dragstart: {
				GetCursorPos(&offset);

				RECT rect{ };
				GetWindowRect(window.wnd_handle, &rect);
				offset.x -= rect.left;
				offset.y -= rect.top;
			} break;
		}
	}

	void initialize() {
		if(!(context = Rml::CreateContext("main", window.size)))
			utils::logger.log(utils::e_log_type::error, "Rml::CreateContext return nullptr");

		if(document = context->LoadDocument("[resource:rml] main.rml")) {
			document->Show();
			document->GetElementById("titlebar")->AddEventListener(Rml::EventId::Handledrag, &event_listener);
			document->GetElementById("titlebar")->AddEventListener(Rml::EventId::Dragstart, &event_listener);
		}
	}
}