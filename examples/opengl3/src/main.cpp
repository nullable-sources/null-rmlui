#define RMLUI_STATIC_LIB
#define null_renderer_use_glfw
#include <null-rmlui-renderer-opengl3.h>
#include <null-render-backend-opengl3.h>
#include <null-rmlui.h>

Rml::Context* context{ };
null::render::opengl3::c_window window{ };
utils::c_cumulative_time_measurement frame_counter{ 60 };

void main_loop() {
    null::render::begin_frame(); {
        std::shared_ptr<null::render::c_sdf_brush> text_brush = null::render::c_sdf_brush::instance();
        text_brush->set_size(30.f);
        text_brush->set_outline_blur(1.f);
        text_brush->set_outline_color(color_t<int>(100, 100, 255));
        text_brush->set_outline_width(2.f);
        null::render::draw_list->add_text(std::format("[ opengl3 ] fps: {:3.0f}", 1.f / std::chrono::duration<float>{ frame_counter.representation() }.count()), { }, text_brush);

        context->Update();
    } null::render::end_frame();

    null::render::backend::renderer->begin_render();
    null::rml::render_interface->begin_render();
    context->Render();
    null::rml::render_interface->end_render();
    null::render::backend::renderer->end_render();
}

int main() {
    window = null::render::opengl3::c_window{ };
    window.size = { 1024, 768 };

    window.callbacks.at<utils::win::e_window_callbacks::on_create>().add([&] { frame_counter.begin(); });
    window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add([&] { frame_counter.update(); });

    window.callbacks.at<utils::win::e_window_callbacks::on_main_loop>().add(main_loop);

    try {
        window.create();

        null::render::font_config_t config{ };
        config.load_font_default()
            .set_render_mode(null::render::e_font_render_mode::sdf)
            .set_pixel_range(2.f)
            .set_size(14.f);

        null::render::atlas.font_loader = std::make_unique<null::render::c_freetype_loader>();
        null::render::atlas.add_font(config);

        null::rml::render_interface = std::make_unique<null::rml::opengl3::c_render>();
        null::rml::set_default_interfaces(window);
        null::rml::initialize();
        null::rml::load_system_font();

        if(!(context = Rml::CreateContext("main", window.size)))
            utils::logger(utils::e_log_type::error, "Rml::CreateContext return nullptr");

        if(Rml::ElementDocument* document{ context->LoadDocument("[resource:rml] test.rml") })
            document->Show();

        window.main_loop();

        null::rml::shutdown();

        window.destroy();
    } catch(const std::exception& exception) {
        std::cout << exception.what() << std::endl;
    }
}