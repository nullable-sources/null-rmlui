#pragma once
#include <RmlUi/Core.h>
#include <null-sdk.h>

namespace null::rml {
	class c_system_interface : public Rml::SystemInterface {
	public:
		HWND wnd_handle{ };

	private:
		utils::c_immediate_time_measurement time_measurement{ };

		std::unordered_map<std::string, HCURSOR> cursors{
			{ "arrow",			LoadCursor(nullptr, IDC_ARROW) },
			{ "move",			LoadCursor(nullptr, IDC_SIZEALL) },
			{ "pointer",		LoadCursor(nullptr, IDC_HAND) },
			{ "resize",			LoadCursor(nullptr, IDC_SIZENWSE) },
			{ "cross",			LoadCursor(nullptr, IDC_CROSS) },
			{ "text",			LoadCursor(nullptr, IDC_IBEAM) },
			{ "unavailable",	LoadCursor(nullptr, IDC_NO) }
		};

	public:
		c_system_interface(const utils::win::c_window& window) : wnd_handle{ window.wnd_handle } { time_measurement.begin(); }

	public:
		double GetElapsedTime() override { return std::chrono::duration<double>{ time_measurement.representation() }.count(); }

		void SetMouseCursor(const std::string& cursor_name) override;

		void SetClipboardText(const std::string& text) override;
		void GetClipboardText(std::string& text) override;

		void ActivateKeyboard(Rml::Vector2f caret_position, float line_height) override;
	}; inline std::unique_ptr<c_system_interface> system_interface{ };
}