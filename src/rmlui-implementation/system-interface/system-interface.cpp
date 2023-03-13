#pragma comment(lib, "imm32")
#include <rmlui-implementation/backend/backend.h>

namespace null::rml {
	void c_system_interface::SetMouseCursor(const std::string& cursor_name) {
		if(!wnd_handle) return;

		HCURSOR cursor{ cursors[cursor_name.empty() ? "arrow" : cursor_name] };
		if(!cursor) return;

		SetCursor(cursor);
		SetClassLongPtrA(wnd_handle, GCLP_HCURSOR, (LONG_PTR)cursor);
	}

	void c_system_interface::SetClipboardText(const std::string& text_utf8) {
		if(!wnd_handle) throw std::runtime_error{ "wnd_handle empty" };

		if(!OpenClipboard(wnd_handle))
			return;

		EmptyClipboard();

		std::wstring text{ backend::convert_to_utf16(text_utf8) };
		size_t size{ sizeof(wchar_t) * (text.size() + 1) };

		HGLOBAL clipboard_data{ GlobalAlloc(GMEM_FIXED, size) };
		memcpy(clipboard_data, text.data(), size);

		if(!SetClipboardData(CF_UNICODETEXT, clipboard_data)) {
			CloseClipboard();
			GlobalFree(clipboard_data);
		} else CloseClipboard();
	}

	void c_system_interface::GetClipboardText(std::string& text) {
		if(!wnd_handle) throw std::runtime_error{ "wnd_handle empty" };

		if(!OpenClipboard(wnd_handle))
			return;

		HANDLE clipboard_data{ GetClipboardData(CF_UNICODETEXT) };
		if(!clipboard_data) {
			CloseClipboard();
			return;
		}

		const wchar_t* clipboard_text{ (const wchar_t*)GlobalLock(clipboard_data) };
		if(clipboard_text) text = backend::convert_to_utf8(clipboard_text);
		GlobalUnlock(clipboard_data);

		CloseClipboard();
	}

	void c_system_interface::ActivateKeyboard(Rml::Vector2f caret_position, float /*line_height*/) {
		if(HIMC himc{ ImmGetContext(wnd_handle) }) {
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
			case Rml::Log::LT_ALWAYS: { std::cout << "\033[1;47;30m[always | rmlui]\033[0m \x1B[37m" << message << "\033[0m" << std::endl; } break;
			case Rml::Log::LT_ERROR: { std::cout << "\033[3;42;30m[error | rmlui]\033[0m \x1B[31m" << message << "\033[0m" << std::endl; } break;
			case Rml::Log::LT_WARNING: { std::cout << "\033[3;43;30m[warning | rmlui]\033[0m \x1B[33m" << message << "\033[0m" << std::endl; } break;
			case Rml::Log::LT_INFO: { std::cout << "\033[3;104;30m[info | rmlui]\033[0m \x1B[36m" << message << "\033[0m" << std::endl; } break;
			case Rml::Log::LT_DEBUG: { std::cout << "\033[3;47;30m[debug | rmlui]\033[0m \x1B[37m" << message << "\033[0m" << std::endl; } break;
			default: { return SystemInterface::LogMessage(log_type, message); } break;
		}
		return true;
	}
}