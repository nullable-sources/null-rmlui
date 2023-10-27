#pragma once
#include <RmlUi/Core.h>

namespace null::rml::extensions {
	//@note: you style the necessary colorpicker yourself, an example will be later
	class i_widget_colorpicker : public Rml::EventListener {
	protected:
		Rml::Element* control_element{ };

		Rml::Element* canvas{ }, *indicator{ }; //@note: indicator should not be part of DOM

		Rml::Element* preview{ };

		Rml::Element* hue_slider{ }, *alpha_slider{ };
		Rml::Element* value_input{ }, *copy_button{ }, *paste_button{ }; //@note: these elements are optional, but there is functionality for them, so just create them to use

		bool value_dirty{ true }, value_from_input{ }, indicator_dirty{ };
		bool lock_value{ };

	protected:
		virtual void add_events();
		virtual void remove_events();

		virtual void set_elements_color(const color_t<int>& color) { } //@note: here you update the colors of the elements you need, such as preview, canvas, shoe slider, alpha slider, etc.

	protected:
		void on_update();
		void format_elements();

		void set_elements_value(const std::string& value);
		void on_value_change(const std::string& value);

		color_t<int> build_color() const { return color_t<int>(hsv_color_t(hue_slider->GetAttribute("value", 0.f), canvas->GetAttribute("saturation", 0.f), canvas->GetAttribute("brightness", 0.f), alpha_slider->GetAttribute("value", 0.f)); }
		std::string format_to_hex(const color_t<int>& color) const { return std::format("#{:02X}{:02X}{:02X}{:02X}", color.r, color.g, color.b, color.a); }

	public:
		void ProcessEvent(Rml::Event& event) override;
	};
}