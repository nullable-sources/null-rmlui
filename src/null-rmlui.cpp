#include "null-rmlui.h"

namespace ntl::rml {
    void set_default_interfaces(const win::c_window& window) {
        file_interface = std::make_unique<c_file_interface>();
        system_interface = std::make_unique<c_system_interface>(window);
        renderer::c_filter_factory::register_default_instancers();
    }

    void load_system_font() {
        Rml::LoadFontFace("{system-fonts}\\Tahoma.ttf", true);
    }

    void initialize() {
        if(file_interface) Rml::SetFileInterface(file_interface.get());
        else utils::logger(utils::e_log_type::error, "file_interface is empty.");

        if(system_interface) Rml::SetSystemInterface(system_interface.get());
        else utils::logger(utils::e_log_type::error, "system_interface is empty.");

        if(render_interface) Rml::SetRenderInterface(render_interface.get());
        else utils::logger(utils::e_log_type::error, "system_interface is empty.");

        renderer::layers = std::make_unique<renderer::c_layers>();
        renderer::layers->initialize();
        render_interface->initialize();

        if(!Rml::Initialise()) utils::logger(utils::e_log_type::error, "Rml::Initialise return false.");
    }

    void shutdown() {
        Rml::Shutdown();

        render_interface->shutdown();

        render_interface.reset();
        system_interface.reset();
        file_interface.reset();
    }
}