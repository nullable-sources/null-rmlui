#include <null-rmlui-renderer-directx11.h>
#include <null-rmlui.h>

#include <RmlUi/Lottie/LottiePlugin.h>
#include <RmlUi/SVG/SVGPlugin.h>
#include <RmlUi/Debugger.h>

Rml::Context* context{ };
ntl::render::directx11::c_window window{ };
ntl::c_cumulative_time_measurement frame_counter{ 60 };

void add_gemometry(const std::vector<vec2_t<float>>& points, std::shared_ptr<ntl::render::c_brush>& brush) {
    auto mesh = ntl::render::backend::factory->instance_mesh();
    auto draw_list = ntl::render::c_draw_list::instance(std::move(mesh));
    draw_list->add_convex_shape(points, brush);
    draw_list->compile();
    //ntl::rml::render_interface->command_buffer.add_command(std::move(draw_list));
}

void main_loop() {
    ntl::render::begin_frame(); {
        std::shared_ptr<ntl::render::c_sdf_brush> text_brush = ntl::render::c_sdf_brush::instance();
        text_brush->set_size(30.f);
        text_brush->set_outline_blur(1.f);
        text_brush->set_outline_color(color_t<int>(100, 100, 255));
        text_brush->set_outline_width(2.f);
        ntl::render::draw_list->add_text(std::format("[ directx11 ] fps: {:3.0f}", 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count()), { }, text_brush);

        context->Update();
    } ntl::render::end_frame();

    ntl::render::backend::renderer->begin_render();
    ntl::rml::render_interface->begin_render();
    context->Render();
    ntl::rml::render_interface->end_render();
    ntl::render::backend::renderer->end_render();
}


int main() {
    window = ntl::render::directx11::c_window{ };

    window.callbacks.at<ntl::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
    window.callbacks.at<ntl::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

    window.callbacks.at<ntl::win::e_window_callbacks::on_main_loop>().add(main_loop);
    window.callbacks.at<ntl::win::e_window_callbacks::on_wnd_proc>().add([&](HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) { return ntl::rml::backend::wnd_proc(context, hwnd, msg, w_param, l_param); });

    try {
        window.create();

        ntl::render::font_config_t config{ };
        config.load_font_default()
              .set_render_mode(ntl::render::e_font_render_mode::sdf)
              .set_pixel_range(2.f)
              .set_size(14.f);

        ntl::render::atlas.font_loader = std::make_unique<ntl::render::c_freetype_loader>();
        ntl::render::atlas.add_font(config);

        ntl::rml::render_interface = std::make_unique<ntl::rml::directx11::c_render>();
        ntl::rml::set_default_interfaces(window);
        ntl::rml::initialize();
        ntl::rml::load_system_font();

        if(!(context = Rml::CreateContext("main", window.size)))
            ntl::sdk::logger(ntl::sdk::e_log_type::error, "Rml::CreateContext return nullptr");

        Rml::SVG::Initialise();
        Rml::Lottie::Initialise();
        Rml::Debugger::Initialise(context);

        if(Rml::ElementDocument* document{ context->LoadDocument("[resource:rml] test.rml") })
            document->Show();

        window.main_loop();

        ntl::rml::shutdown();

        window.destroy();
    } catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl;
    }
}