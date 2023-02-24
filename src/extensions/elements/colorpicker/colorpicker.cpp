#include <null-rmlui.h>

namespace null::rml::extensions::elements {
	c_widget_colorpicker::c_widget_colorpicker(Rml::ElementFormControl* element) : parent_element{ element } {
		colorpicker_box = (c_element_form_control_colorpicker_box*)parent_element->AppendChild(Rml::Factory::InstanceElement(parent_element, "colorpicker-box", "box", { }), false);

		colorpicker_box->SetProperty(Rml::PropertyId::Visibility, { Rml::Style::Visibility::Hidden });
		colorpicker_box->SetProperty(Rml::PropertyId::ZIndex, Rml::Property{ 1.0f, Rml::Property::NUMBER });

		colorpicker_box->AddEventListener(Rml::EventId::Change, this);

		parent_element->AddEventListener(Rml::EventId::Click, this, true);
		parent_element->AddEventListener(Rml::EventId::Blur, this);
		parent_element->AddEventListener(Rml::EventId::Focus, this);
		parent_element->AddEventListener(Rml::EventId::Keydown, this, true);
	}

	c_widget_colorpicker::~c_widget_colorpicker() {
		colorpicker_box->RemoveEventListener(Rml::EventId::Change, this);

		parent_element->RemoveEventListener(Rml::EventId::Click, this, true);
		parent_element->RemoveEventListener(Rml::EventId::Blur, this);
		parent_element->RemoveEventListener(Rml::EventId::Focus, this);
		parent_element->RemoveEventListener(Rml::EventId::Keydown, this, true);
	}

	void c_widget_colorpicker::on_update() {
		if(color_dirty) {
			parent_element->SetProperty(Rml::PropertyId::BackgroundColor, { color_t<int>{ colorpicker_box->get_color(), 255 }, Rml::Property::COLOUR });
			color_dirty = false;
		}
	}

	void c_widget_colorpicker::on_render() {
		if(!box_layout_dirty || !box_visible) return;

		Rml::Box box{ };
		Rml::ElementUtilities::BuildBox(box, parent_element->GetBox().GetSize(), colorpicker_box);
		Rml::ElementUtilities::FormatElement(colorpicker_box, parent_element->GetBox().GetSize(Rml::Box::BORDER));
		colorpicker_box->SetOffset(Rml::Vector2f{ parent_element->GetBox().GetSize(Rml::Box::BORDER).x + box.GetEdge(Rml::Box::MARGIN, Rml::Box::LEFT), parent_element->GetBox().GetSize(Rml::Box::BORDER).y / 2.f + box.GetEdge(Rml::Box::MARGIN, Rml::Box::TOP) - box.GetSize().y / 2 }, parent_element);

		box_layout_dirty = false;
	}

	void c_widget_colorpicker::on_layout() {
		Rml::ElementUtilities::PositionElement(colorpicker_box, Rml::Vector2f{ 0, 0 }, Rml::ElementUtilities::TOP_LEFT);

		box_layout_dirty = true;
	}
	
	void c_widget_colorpicker::ProcessEvent(Rml::Event& event) {
		if(parent_element->IsDisabled())
			return;

		switch(event.GetId()) {
			case Rml::EventId::Change: {
				color_dirty = true;
				update_value();
			}

			case Rml::EventId::Click: {
				Rml::Element* element{ event.GetTargetElement() };
				while(element && element != parent_element) {
					if(element == colorpicker_box) return;
					element = element->GetParentNode();
				}

				show_colorpicker_box(colorpicker_box->GetComputedValues().visibility() == Rml::Style::Visibility::Hidden);
			} break;

			case Rml::EventId::Focus: {
				if(event.GetTargetElement() == parent_element) {
					parent_element->SetPseudoClass("focus", true);
				}
			} break;
			case Rml::EventId::Blur: {
				if(event.GetTargetElement() == parent_element) {
					show_colorpicker_box(false);
					parent_element->SetPseudoClass("focus", false);
				}
			} break;
		}
	}
	
	void c_widget_colorpicker::on_value_change(const std::string& value) {
		if(!lock_value) colorpicker_box->SetValue(value);

		parent_element->DispatchEvent(Rml::EventId::Change, { { "value", Rml::Variant{ value }  } });
	}

	void c_widget_colorpicker::show_colorpicker_box(const bool& show) {
		if(show) {
			color_dirty = true;
			box_layout_dirty = true;
		}
		colorpicker_box->SetProperty(Rml::PropertyId::Visibility, { show ? Rml::Style::Visibility::Visible : Rml::Style::Visibility::Hidden });
		parent_element->SetPseudoClass("checked", show);
		box_visible = show;
	}

	void c_widget_colorpicker::update_value() {
		lock_value = true;
		parent_element->SetAttribute("value", colorpicker_box->GetAttribute("value", std::string{ }));
		lock_value = false;
	}

	void c_element_form_control_colorpicker::OnAttributeChange(const Rml::ElementAttributes& changed_attributes) {
		RMLUI_ASSERT(widget);

		ElementFormControl::OnAttributeChange(changed_attributes);

		if(const auto& it{ changed_attributes.find("value") }; it != changed_attributes.end())
			widget->on_value_change(it->second.Get<std::string>());
	}
}