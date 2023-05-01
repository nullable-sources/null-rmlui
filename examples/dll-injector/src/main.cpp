#define RMLUI_STATIC_LIB
#pragma comment(lib, "dwmapi.lib")
#include <dwmapi.h>


#include "lua/lua.h"
#include "ui/ui.h"
utils::c_cumulative_time_measurement frame_counter{ 60 };
int main() {
#ifndef _DEBUG
	ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
	ui::window = null::render::backend::directx11::c_window{ };
	ui::window.wnd_class.hIcon = (HICON)LoadImage(GetModuleHandleA(nullptr), L"icon.ico", IMAGE_ICON, 256, 256, LR_DEFAULTCOLOR);
	ui::window.size = { 560, 300 };
	ui::window.clear_color = { 0, 0 };
	ui::window.wnd_class.style = 0x2;
	ui::window.styles = WS_POPUP;

	ui::window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
	ui::window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

	ui::window.callbacks.at<utils::win::e_window_callbacks::on_wnd_proc>().add([&](HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) { return null::rml::backend::wnd_proc(ui::context, hwnd, msg, w_param, l_param); });
	ui::window.callbacks.at<utils::win::e_window_callbacks::on_wnd_proc>().add([](HWND, UINT msg, WPARAM, LPARAM) { if(msg == WM_SIZE) Rml::ReleaseTextures(); return -1; });
	ui::window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&]() {
		null::render::begin_frame(ui::window); {
			ui::context->Update();
			ui::context->Render();
		} null::render::end_frame();

		null::render::backend::renderer->begin_render();
		null::rml::render_interface->render();
		null::render::backend::renderer->end_render();
		});

	try {
		ui::window.create();

		HRGN region{ CreateRectRgn(0, 0, -1, -1) };
		DWM_BLURBEHIND blur_behind{
			.dwFlags{ DWM_BB_ENABLE | DWM_BB_BLURREGION },
			.fEnable{ TRUE },
			.hRgnBlur{ region }
		};
		DwmEnableBlurBehindWindow(ui::window.wnd_handle, &blur_behind);
		DeleteObject(region);

		null::rml::render_interface = std::make_unique<null::rml::renderer::c_directx11>();
		null::rml::set_default_interfaces(ui::window);
		null::rml::initialize();
		null::rml::load_system_font();

		null::rml::extensions::decorators::register_all();
		null::rml::extensions::elements::register_all();

		lua::initialize();
		ui::initialize();

		ui::window.main_loop();

		null::rml::shutdown();

		ui::window.destroy();
	} catch(const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
}