#define null_renderer_use_glfw
#include <null-rmlui-renderer-opengl3.h>
#include <null-rmlui.h>

WNDPROC old_wnd_proc{ };

Rml::Context* context{ };
null::renderer::c_window window{ };
utils::c_cumulative_time_measurement frame_counter{ 60 };

void main_loop() {
	null::render::begin_frame(window); {
		null::render::background.draw_text(std::format("[ opengl3 ] fps: {:3.0f}", 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count()), { window.get_window_size().x, 10.f }, { }, null::render::e_text_flags{ -null::render::e_text_flags::aligin_right | -null::render::e_text_flags::aligin_center_y | -null::render::e_text_flags::outline });

		context->Update();
		context->Render();
	} null::render::end_frame();
}

static LRESULT CALLBACK win_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	if(int result{ null::rml::backend::wnd_proc(context, hwnd, msg, w_param, l_param) }; result != -1) return result;
	return CallWindowProc(old_wnd_proc, window.wnd_handle, msg, w_param, l_param);
}

int main() {
	window = null::renderer::c_window{ };
	window.size = { 1024, 768 };

	window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);

	try {
		window.create();

		null::render::c_font::config_t config{ .glyph_config{ .ranges{ null::render::c_font::glyph_t::ranges_cyrillic() } } };
		null::render::atlas.add_font_default(&config);

		null::rml::render_interface = std::make_unique<null::rml::renderer::c_opengl3>();
		null::rml::set_default_interfaces(window);
		null::rml::initialize();
		null::rml::load_system_font();

		if(!(context = Rml::CreateContext("main", { window.size.x, window.size.y })))
			throw std::runtime_error{ "Rml::CreateContext return nullptr" };

		if(Rml::ElementDocument * document{ context->LoadDocument("<resource:rml> tutorial.rml") })
			document->Show();
		
		old_wnd_proc = (WNDPROC)GetWindowLongPtr(window.wnd_handle, GWL_WNDPROC);
		SetWindowLongPtr(window.wnd_handle, GWL_WNDPROC, (long)win_proc);

		window.main_loop();
		window.destroy();

		Rml::Shutdown();
		null::rml::shutdown();
	} catch(const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
}