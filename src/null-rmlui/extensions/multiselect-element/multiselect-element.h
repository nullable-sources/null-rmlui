#pragma once
#include <RmlUi/Core.h>

namespace null::rml::extensions {
	class c_widget_multi_dropdown : public Rml::EventListener {
	protected:
		Rml::ElementFormControl* parent_element{ };

		Rml::Element* button_element{ };
		Rml::Element* selection_element{ };
		Rml::Element* value_element{ };

		bool lock_selection{ };
		bool selection_dirty{ };
		bool value_rml_dirty{ };
		bool value_layout_dirty{ };
		bool box_layout_dirty{ };
		bool box_visible{ };

	public:
		c_widget_multi_dropdown(Rml::ElementFormControl* element);
		virtual ~c_widget_multi_dropdown();

	public:
		void on_update();
		void on_render();
		void on_layout();

		void on_child_add(Rml::Element* element);
		void on_child_remove(Rml::Element* element);
		void ProcessEvent(Rml::Event& event) override;

		void on_value_change(const std::string& value);
		
		int add_option(const std::string& rml, const std::string& value, const int& before, const bool& select, const bool& selectable = true);
		int add_option(Rml::ElementPtr element, const int& before);
		
		void remove_option(const int& index);
		void clear_options();

		void switch_selection(Rml::Element* option_element);
		void set_selection(Rml::Element* option_element, const bool& selection);

		Rml::Element* get_option(const int& index) { return selection_element->GetChild(index); }
		int get_num_options() const { return selection_element->GetNumChildren(); }
		std::vector<Rml::Element*> get_selections() const {
			return std::views::iota(0, selection_element->GetNumChildren())
				| std::views::transform([&](const int& i) { return selection_element->GetChild(i); })
				| std::views::filter([](const Rml::Element* element) { return element->HasAttribute("selected"); })
				| std::ranges::to<std::vector>();
		}

	private:
		void update_value();

		void show_selectbox(const bool& show);

		void attach_scroll_event();
		void detach_scroll_event();
	};

	class c_element_form_control_multiselect : public Rml::ElementFormControl {
	public:
		static inline Rml::ElementInstancerGeneric<c_element_form_control_multiselect> instancer{ };
		
		static void register_instancer() { Rml::Factory::RegisterElementInstancer("multiselect", &instancer); }

	protected:
		c_widget_multi_dropdown* widget{ };

	public:
		c_element_form_control_multiselect(const std::string& tag) : Rml::ElementFormControl{ tag }, widget{ new c_widget_multi_dropdown{ this } } { }
		virtual ~c_element_form_control_multiselect() { delete widget; }

	public:
		std::string GetValue() const override { return GetAttribute("value", std::string{ }); }
		void SetValue(const std::string& value) override { move_children(); SetAttribute("value", value); }

		int add(const std::string& rml, const std::string& value, const int& before = -1, const bool& selectable = true) { move_children(); return widget->add_option(rml, value, before, false, selectable); }
		int add(Rml::ElementPtr element, const int& before = -1) { move_children(); return widget->add_option(std::move(element), before); }
		
		void remove(const int& index) { move_children(); widget->remove_option(index); }
		void remove_all() { move_children(); widget->clear_options(); }
		
		void switch_selection(const int& index) { move_children(); widget->switch_selection(get_option(index)); }
		void set_selection(const int& index, const bool& selection) { move_children(); widget->set_selection(get_option(index), selection); }

		Rml::Element* get_option(const int& index) { move_children(); return widget->get_option(index); }
		int get_num_options() { move_children(); return widget->get_num_options(); }
		std::vector<Rml::Element*> get_selections() { move_children(); return widget->get_selections(); }

	protected:
		void OnLayout() override { widget->on_layout(); }
		void OnRender() override { Rml::ElementFormControl::OnRender(); widget->on_render(); }
		void OnUpdate() override { Rml::ElementFormControl::OnUpdate(); move_children(); widget->on_update(); }

		void OnChildAdd(Rml::Element* child) override { if(widget) widget->on_child_add(child); }
		void OnChildRemove(Rml::Element* child) override { if(widget) widget->on_child_remove(child); }

		bool GetIntrinsicDimensions(Rml::Vector2f& intrinsic_dimensions, float& intrinsic_ratio) override { intrinsic_dimensions = { 128, 16 }; return true; }

		void OnAttributeChange(const Rml::ElementAttributes& changed_attributes) override;

		void move_children();
	};
}