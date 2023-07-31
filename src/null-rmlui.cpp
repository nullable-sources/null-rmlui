#include <null-rmlui.h>

namespace null::rml {
	void set_default_interfaces(const utils::win::c_window& window) {
		file_interface = std::make_unique<rml::c_file_interface>();
		system_interface = std::make_unique<rml::c_system_interface>(window);
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