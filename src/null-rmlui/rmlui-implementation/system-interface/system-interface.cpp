#pragma comment(lib, "imm32")
#include "../backend/backend.h"

namespace null::rml {
	void c_system_interface::SetMouseCursor(const std::string& cursor_name) {
		if(!wnd_handle) return;

		HCURSOR cursor = cursors[cursor_name.empty() ? "arrow" : cursor_name];
		if(!cursor) return;

		SetCursor(cursor);
		SetClassLongPtrA(wnd_handle, GCLP_HCURSOR, (LONG_PTR)cursor);
	}

	void c_system_interface::SetClipboardText(const std::string& text_utf8) {
		if(!wnd_handle) { utils::logger(utils::e_log_type::warning, "wnd_handle is empty."); return; }

		if(!OpenClipboard(wnd_handle))
			return;

		EmptyClipboard();

		std::wstring text = backend::convert_to_utf16(text_utf8);
		size_t size = sizeof(wchar_t) * (text.size() + 1);

		HGLOBAL clipboard_data = GlobalAlloc(GMEM_FIXED, size);
		memcpy(clipboard_data, text.data(), size);

		if(!SetClipboardData(CF_UNICODETEXT, clipboard_data)) {
			CloseClipboard();
			GlobalFree(clipboard_data);
		} else CloseClipboard();
	}

	void c_system_interface::GetClipboardText(std::string& text) {
		if(!wnd_handle) { utils::logger(utils::e_log_type::warning, "wnd_handle is empty."); return; }

		if(!OpenClipboard(wnd_handle))
			return;

		HANDLE clipboard_data = GetClipboardData(CF_UNICODETEXT);
		if(!clipboard_data) {
			CloseClipboard();
			return;
		}

		const wchar_t* clipboard_text = (const wchar_t*)GlobalLock(clipboard_data);
		if(clipboard_text) text = backend::convert_to_utf8(clipboard_text);
		GlobalUnlock(clipboard_data);

		CloseClipboard();
	}

	void c_system_interface::ActivateKeyboard(Rml::Vector2f caret_position, float /*line_height*/) {
		if(HIMC himc = ImmGetContext(wnd_handle)) {
			COMPOSITIONFORM comp{
				.dwStyle{ CFS_FORCE_POSITION },
				.ptCurrentPos{ (LONG)caret_position.x, (LONG)caret_position.y }
			};
			ImmSetCompositionWindow(himc, &comp);

			CANDIDATEFORM cand{
				.dwStyle{ CFS_CANDIDATEPOS },
				.ptCurrentPos{ (LONG)caret_position.x, (LONG)caret_position.y }
			};
			ImmSetCandidateWindow(himc, &cand);

			ImmReleaseContext(wnd_handle, himc);
		}
	}

	bool c_system_interface::LogMessage(Rml::Log::Type log_type, const std::string& message) {
		switch(log_type) {
			case Rml::Log::LT_INFO:
			case Rml::Log::LT_DEBUG:
			case Rml::Log::LT_ALWAYS: { utils::logger(utils::e_log_type::info, "rmlui | {}", message); } break;

			case Rml::Log::LT_ERROR: { utils::logger(utils::e_log_type::error, "rmlui | {}", message); } break;

			case Rml::Log::LT_WARNING: { utils::logger(utils::e_log_type::warning, "rmlui | {}", message); } break;
			default: { return SystemInterface::LogMessage(log_type, message); } break;
		}
		return true;
	}
}