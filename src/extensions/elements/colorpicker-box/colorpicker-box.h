#pragma once
#include <RmlUi/Core.h>

#include <null-sdk.h>

namespace null::rml::extensions::elements {
	class c_widget_colorpicker_box : public Rml::EventListener {
	protected:
		Rml::ElementFormControl* parent_element{ };

		Rml::Element* picker_content{ };
		Rml::Element* picker_box{ };
		Rml::Element* picker_indicator{ };

		Rml::Element* inline_block_wrapper{ };
		Rml::Element* preview_content{ };
		Rml::Element* preview_flex_wrapper{ };
		Rml::Element* preview_circle{ };

		Rml::Element* sliders_content{ };
		Rml::Element* sliders_flex_wrapper{ };
		Rml::Element* hue_slider{ };
		Rml::Element* alpha_slider{ };

		bool update_picker_and_preview{ true };
		bool lock_value{ };

	public:
		c_widget_colorpicker_box(Rml::ElementFormControl* element);
		virtual ~c_widget_colorpicker_box();

	public:
		void on_update();
		void on_render();

		void ProcessEvent(Rml::Event& event) override;

		void on_value_change(const std::string& value);

		color_t<int> build_color() const { return color_t<int>{ hsv_color_t{ hue_slider->GetAttribute("value", 0.f), picker_box->GetAttribute("saturation", 0.f), picker_box->GetAttribute("brightness", 0.f), alpha_slider->GetAttribute("value", 0.f) } }; }
		bool alpha_slider_visible() const { return alpha_slider->IsVisible(); }

	private:
		void update_value();
	};

	class c_element_form_control_colorpicker_box : public Rml::ElementFormControl {
	public:
		static inline Rml::ElementInstancerGeneric<c_element_form_control_colorpicker_box> instancer{ };

		static void register_instancer() { Rml::Factory::RegisterElementInstancer("colorpicker-box", &instancer); }

	protected:
		c_widget_colorpicker_box* widget{ };

	public:
		c_element_form_control_colorpicker_box(const std::string& tag) : Rml::ElementFormControl{ tag }, widget{ new c_widget_colorpicker_box{ this } } { }
		virtual ~c_element_form_control_colorpicker_box() { delete widget; }

	public:
		std::string GetValue() const override { return GetAttribute("value", std::string{ }); }
		void SetValue(const std::string& value) override { SetAttribute("value", value); }

		color_t<int> get_color() const { return widget->build_color(); }
		bool alpha_slider_visible() const { return widget->alpha_slider_visible(); }

	protected:
		void OnRender() override { Rml::ElementFormControl::OnRender(); widget->on_render(); }
		void OnUpdate() override { Rml::ElementFormControl::OnUpdate(); widget->on_update(); }

		bool GetIntrinsicDimensions(Rml::Vector2f& intrinsic_dimensions, float& intrinsic_ratio) override { intrinsic_dimensions = { 200, 170 }; intrinsic_ratio = 1; return true; }

		void OnAttributeChange(const Rml::ElementAttributes& changed_attributes) override;
	};
}