﻿#define RMLUI_STATIC_LIB
#include "lua/lua.h"
#include "ui/ui.h"

int main() {
#ifndef _DEBUG
	ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
	ui::window = null::renderer::c_window{ };
	ui::window.wnd_class.hIcon = (HICON)LoadImage(GetModuleHandleA(nullptr), L"icon.ico", IMAGE_ICON, 256, 256, LR_DEFAULTCOLOR);
	ui::window.size = { 300, 254 };
	ui::window.wnd_class.style = 0x2;
	ui::window.styles = WS_POPUP;

	ui::window.callbacks.at<utils::win::e_window_callbacks::on_wnd_proc>().add([&](HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) { return null::rml::backend::wnd_proc(ui::context, hwnd, msg, w_param, l_param); });
	ui::window.callbacks.at<utils::win::e_window_callbacks::on_wnd_proc>().add([](HWND, UINT msg, WPARAM, LPARAM) { if(msg == WM_SIZE) Rml::ReleaseTextures(); return -1; });
	ui::window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&]() {
		null::render::begin_frame(ui::window); {
			ui::context->Update();
			ui::context->Render();
		} null::render::end_frame();
		});

	try {
		ui::window.create();
		
		null::rml::render_interface = std::make_unique<null::rml::renderer::c_directx9>();
		null::rml::set_default_interfaces(ui::window);
		null::rml::initialize();
		null::rml::load_system_font();

		lua::initialize();
		ui::initialize();

		ui::window.main_loop();

		null::rml::shutdown();

		ui::window.destroy();
	} catch(const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
}