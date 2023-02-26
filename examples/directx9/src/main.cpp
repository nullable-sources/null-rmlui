#define RMLUI_STATIC_LIB
#include <null-rmlui-renderer-directx9.h>
#include <null-rmlui.h>

Rml::Context* context{ };
null::renderer::c_window window{ };
utils::c_cumulative_time_measurement frame_counter{ 60 };

void main_loop() {
	null::render::begin_frame(window); {
		null::render::background.add_text(std::format("[ directx9 ] fps: {:3.0f}", 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count()), { (float)window.get_window_size().x, 10.f }, { }, null::render::e_text_flags{ -null::render::e_text_flags::aligin_right | -null::render::e_text_flags::aligin_center_y | -null::render::e_text_flags::outline });

		context->Update();
		context->Render();
	} null::render::end_frame();
}


int main() {
	window = null::renderer::c_window{ };
	window.size = { 1024, 768 };

	window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

	window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);
	window.callbacks.at<utils::win::e_window_callbacks::on_wnd_proc>().add([&](HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) { return null::rml::backend::wnd_proc(context, hwnd, msg, w_param, l_param); });
	window.callbacks.at<utils::win::e_window_callbacks::on_wnd_proc>().add([](HWND, UINT msg, WPARAM, LPARAM) { if(msg == WM_SIZE) Rml::ReleaseTextures(); return -1; });

	try {
		window.create();

		null::render::c_font::config_t config{ .glyph_config{ .ranges{ null::render::c_font::glyph_t::ranges_cyrillic() } } };
		null::render::atlas.add_font_default(&config);

		null::rml::render_interface = std::make_unique<null::rml::renderer::c_directx9>();
		null::rml::set_default_interfaces(window);
		null::rml::initialize();
		null::rml::load_system_font();

		if(!(context = Rml::CreateContext("main", window.size)))
			throw std::runtime_error{ "Rml::CreateContext return nullptr" };

		if(Rml::ElementDocument* document{ context->LoadDocument("<resource:rml> tutorial.rml") })
			document->Show();

		window.main_loop();

		null::rml::shutdown();

		window.destroy();
	} catch(const std::exception& exception) {
		std::cout << exception.what() << std::endl;
	}
}