#pragma once
#include <RmlUi/Core.h>


#include <null-sdk.h>

namespace null::rml::extensions::elements {
	class c_element_form_control_colorpicker_box;
	class c_widget_colorpicker : public Rml::EventListener {
	protected:
		Rml::ElementFormControl* parent_element{ };

		c_element_form_control_colorpicker_box* colorpicker_box{ };

		bool color_dirty{ };
		bool box_layout_dirty{ };
		bool lock_value{ };
		bool box_visible{ };

	public:
		c_widget_colorpicker(Rml::ElementFormControl* element);
		virtual ~c_widget_colorpicker();

	public:
		void on_update();
		void on_render();
		void on_layout();

		void ProcessEvent(Rml::Event& event) override;

		void on_value_change(const std::string& value);

	private:
		void show_colorpicker_box(const bool& show);
		void update_value();
	};

	class c_element_form_control_colorpicker : public Rml::ElementFormControl {
	public:
		static inline Rml::ElementInstancerGeneric<c_element_form_control_colorpicker> instancer{ };

		static void register_instancer() { Rml::Factory::RegisterElementInstancer("colorpicker", &instancer); }

	protected:
		c_widget_colorpicker* widget{ };

	public:
		c_element_form_control_colorpicker(const std::string& tag) : Rml::ElementFormControl{ tag }, widget{ new c_widget_colorpicker{ this } } { }
		virtual ~c_element_form_control_colorpicker() { delete widget; }

	public:
		std::string GetValue() const override { return GetAttribute("value", std::string{ }); }
		void SetValue(const std::string& value) override { SetAttribute("value", value); }

	protected:
		void OnLayout() override { widget->on_layout(); }
		void OnRender() override { Rml::ElementFormControl::OnRender(); widget->on_render(); }
		void OnUpdate() override { Rml::ElementFormControl::OnUpdate(); widget->on_update(); }

		bool GetIntrinsicDimensions(Rml::Vector2f& intrinsic_dimensions, float& intrinsic_ratio) override { intrinsic_dimensions = { 16, 16 }; intrinsic_ratio = 1; return true; }

		void OnAttributeChange(const Rml::ElementAttributes& changed_attributes) override;
	};
}