#include <null-rmlui.h>

namespace null::rml {
	void set_default_interfaces(const utils::win::c_window& window) {
		file_interface = std::make_unique<null::rml::c_file_interface>();
		system_interface = std::make_unique<null::rml::c_system_interface>(window);
	}

	void load_system_font() {
		Rml::LoadFontFace("{system-fonts}\\Tahoma.ttf", true);
	}

	void initialize() {
		if(file_interface) Rml::SetFileInterface(file_interface.get());
		else throw std::runtime_error{ "file_interface empty" };

		if(system_interface) Rml::SetSystemInterface(system_interface.get());
		else throw std::runtime_error{ "system_interface" };
		
		if(render_interface) Rml::SetRenderInterface(render_interface.get());
		else throw std::runtime_error{ "render_interface empty" };

		if(!Rml::Initialise()) throw std::runtime_error{ "Rml::Initialise return false" };
	}

	void shutdown() {
		Rml::Shutdown();

		render_interface.reset();
		system_interface.reset();
		file_interface.reset();
	}
}