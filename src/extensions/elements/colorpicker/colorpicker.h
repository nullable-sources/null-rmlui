#pragma once
#include <RmlUi/Core.h>

#include <null-sdk.h>

namespace null::rml::extensions::elements {
	//@note: you style the necessary colorpicker yourself, an example will be later
	class i_widget_colorpicker : public Rml::EventListener {
	protected:
		Rml::ElementFormControl* parent_element{ };

		Rml::Element* canvas{ }, *indicator{ }; //@note: indicator should not be part of DOM

		Rml::Element* preview{ };

		Rml::Element* hue_slider{ }, *alpha_slider{ };
		Rml::Element* value_input{ }, *copy_button{ }, *paste_button{ }; //@note: these elements are optional, but there is functionality for them, so just create them to use

		bool value_dirty{ true }, value_from_input{ };
		bool lock_value{ };

	protected:
		virtual void add_events();
		virtual void remove_events();

		virtual void update_colors(const color_t<int>& color) { } //@note: here you update the colors of the elements you need, such as preview, canvas, shoe slider, alpha slider, etc.

	public:
		void on_update();

		void ProcessEvent(Rml::Event& event) override;

		void on_value_change(const std::string_view& value);

		color_t<int> build_color() const { return color_t<int>{ hsv_color_t{ hue_slider->GetAttribute("value", 0.f), canvas->GetAttribute("saturation", 0.f), canvas->GetAttribute("brightness", 0.f), alpha_slider->GetAttribute("value", 0.f) } }; }
		std::string build_color_to_string() const {
			color_t<int> color{ build_color() };
			return std::format("#{:02X}{:02X}{:02X}{:02X}", color.r, color.g, color.b, color.a);
		}

	private:
		void update_value();
	};
}