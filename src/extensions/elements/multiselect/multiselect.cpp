#include <iostream>
#include <windows.h>
#include <format>
#include "multiselect.h"

namespace null::rml::extensions::elements {
	c_widget_multi_dropdown::c_widget_multi_dropdown(Rml::ElementFormControl* element) : parent_element{ element } {
		button_element = parent_element->AppendChild(Rml::Factory::InstanceElement(parent_element, "*", "selectarrow", Rml::XMLAttributes{ }), false);
		value_element = parent_element->AppendChild(Rml::Factory::InstanceElement(parent_element, "*", "selectvalue", Rml::XMLAttributes{ }), false);
		selection_element = parent_element->AppendChild(Rml::Factory::InstanceElement(parent_element, "*", "selectbox", Rml::XMLAttributes{ }), false);

		value_element->SetProperty(Rml::PropertyId::OverflowX, Rml::Property{ Rml::Style::Overflow::Hidden });
		value_element->SetProperty(Rml::PropertyId::OverflowY, Rml::Property{ Rml::Style::Overflow::Hidden });

		selection_element->SetProperty(Rml::PropertyId::Visibility, Rml::Property{ Rml::Style::Visibility::Hidden });
		selection_element->SetProperty(Rml::PropertyId::ZIndex, Rml::Property{ 1.0f, Rml::Property::NUMBER });
		selection_element->SetProperty(Rml::PropertyId::Clip, Rml::Property{ Rml::Style::Clip::Type::None });
		selection_element->SetProperty(Rml::PropertyId::OverflowY, Rml::Property{ Rml::Style::Overflow::Auto });

		parent_element->AddEventListener(Rml::EventId::Click, this, true);
		parent_element->AddEventListener(Rml::EventId::Blur, this);
		parent_element->AddEventListener(Rml::EventId::Focus, this);
		parent_element->AddEventListener(Rml::EventId::Keydown, this, true);

		selection_element->AddEventListener(Rml::EventId::Mousescroll, this);
	}

	c_widget_multi_dropdown::~c_widget_multi_dropdown() {
		for(const int& i : std::views::iota(0, selection_element->GetNumChildren()))
			selection_element->GetChild(i)->RemoveEventListener(Rml::EventId::Click, this);

		parent_element->RemoveEventListener(Rml::EventId::Click, this, true);
		parent_element->RemoveEventListener(Rml::EventId::Blur, this);
		parent_element->RemoveEventListener(Rml::EventId::Focus, this);
		parent_element->RemoveEventListener(Rml::EventId::Keydown, this, true);

		selection_element->RemoveEventListener(Rml::EventId::Mousescroll, this);

		detach_scroll_event();
	}

	void c_widget_multi_dropdown::on_update() {
		if(selection_dirty) {
			std::vector<std::string> select_value{ parent_element->GetAttribute("value", std::string{ }) | std::views::split('&') | std::views::transform([](const auto& range) { return std::string{ range.begin(), range.end() }; }) | std::ranges::to<std::vector>() };

			for(Rml::Element* option : std::views::iota(0, selection_element->GetNumChildren()) | std::views::transform([&](const int& i) { return selection_element->GetChild(i); })) {
				if(option->HasAttribute("selected") || (!select_value.empty() && option->GetAttribute("value", std::string{ }) != std::string{ } && std::ranges::find(select_value, option->GetAttribute("value", std::string{ })) != select_value.end())) {
					set_selection(option, true);
				}
			}

			selection_dirty = false;
		}

		if(value_rml_dirty) {
			if(const std::vector<Rml::Element*>& selections{ get_selections() }; !selections.empty()) value_element->SetInnerRML(selections | std::views::transform([](const Rml::Element* element) { return element->GetInnerRML(); }) | std::views::join_with(std::string_view{ ", " }) | std::ranges::to<std::string>());
			else value_element->SetInnerRML(parent_element->GetValue());

			value_rml_dirty = false;
			value_layout_dirty = true;
		}
	}

	void c_widget_multi_dropdown::on_render() {
		if(box_visible && box_layout_dirty) {
			if(selection_element->GetLocalStyleProperties().count(Rml::PropertyId::Height) == 1) {
				selection_element->RemoveProperty(Rml::PropertyId::Height);
				selection_element->GetOwnerDocument()->UpdateDocument();
			}

			Rml::Box box{ };
			Rml::ElementUtilities::BuildBox(box, parent_element->GetBox().GetSize(), selection_element);

			const float offset_x{ box.GetEdge(Rml::Box::MARGIN, Rml::Box::LEFT) };
			const float offset_y_below{ parent_element->GetBox().GetSize(Rml::Box::BORDER).y + box.GetEdge(Rml::Box::MARGIN, Rml::Box::TOP) };
			const float offset_y_above{ -box.GetEdge(Rml::Box::MARGIN, Rml::Box::BOTTOM) };

			float window_height{ 100'000.f };
			if(Rml::Context* context{ parent_element->GetContext() })
				window_height = float(context->GetDimensions().y);

			const float absolute_y{ parent_element->GetAbsoluteOffset(Rml::Box::BORDER).y };

			const float height_below{ window_height - absolute_y - offset_y_below };
			const float height_above{ absolute_y + offset_y_above };

			Rml::ElementUtilities::FormatElement(selection_element, parent_element->GetBox().GetSize(Rml::Box::BORDER));
			const float content_height{ selection_element->GetOffsetHeight() };

			if(content_height < height_below) selection_element->SetOffset(Rml::Vector2f{ offset_x, offset_y_below }, parent_element);
			else if(content_height < height_above) selection_element->SetOffset(Rml::Vector2f{ offset_x, -content_height + offset_y_above }, parent_element);
			else {
				const float padding_border_size{
					box.GetEdge(Rml::Box::BORDER, Rml::Box::TOP) + box.GetEdge(Rml::Box::BORDER, Rml::Box::BOTTOM) +
					box.GetEdge(Rml::Box::PADDING, Rml::Box::TOP) + box.GetEdge(Rml::Box::PADDING, Rml::Box::BOTTOM)
				};

				float height{ };
				float offset_y{ };

				if(height_below > height_above) {
					height = height_below - padding_border_size;
					offset_y = offset_y_below;
				} else {
					height = height_above - padding_border_size;
					offset_y = offset_y_above - height_above;
				}

				selection_element->SetProperty(Rml::PropertyId::Height, Rml::Property{ height, Rml::Property::PX });
				selection_element->GetOwnerDocument()->UpdateDocument();
				Rml::ElementUtilities::FormatElement(selection_element, parent_element->GetBox().GetSize(Rml::Box::BORDER));

				selection_element->SetOffset(Rml::Vector2f{ offset_x, offset_y }, parent_element);
			}

			box_layout_dirty = false;
		}

		if(value_layout_dirty) {
			Rml::ElementUtilities::FormatElement(value_element, parent_element->GetBox().GetSize(Rml::Box::BORDER));
			value_element->SetOffset(parent_element->GetBox().GetPosition(Rml::Box::CONTENT), parent_element);

			value_layout_dirty = false;
		}
	}

	void c_widget_multi_dropdown::on_layout() {
		if(parent_element->IsDisabled()) {
			value_element->SetPseudoClass("disabled", true);
			button_element->SetPseudoClass("disabled", true);
		}

		Rml::Box parent_box{ parent_element->GetBox() };

		Rml::ElementUtilities::PositionElement(button_element, Rml::Vector2f{ 0, 0 }, Rml::ElementUtilities::TOP_RIGHT);
		Rml::ElementUtilities::PositionElement(selection_element, Rml::Vector2f{ 0, 0 }, Rml::ElementUtilities::TOP_LEFT);

		Rml::Vector2f size{
			parent_element->GetBox().GetSize(Rml::Box::CONTENT).x - button_element->GetBox().GetSize(Rml::Box::MARGIN).x,
			parent_element->GetBox().GetSize(Rml::Box::CONTENT).y
		};

		value_element->SetOffset(parent_element->GetBox().GetPosition(Rml::Box::CONTENT), parent_element);
		value_element->SetBox(Rml::Box{ size });

		box_layout_dirty = true;
		value_layout_dirty = true;
	}

	void c_widget_multi_dropdown::on_child_add(Rml::Element* element) {
		if(element->GetParentNode() != selection_element || element->HasAttribute("data-for") || element->GetTagName() != "option")
			return;

		element->SetProperty(Rml::PropertyId::Display, Rml::Property{ Rml::Style::Display::Block });
		element->SetProperty(Rml::PropertyId::Clip, Rml::Property{ Rml::Style::Clip::Type::Auto });
		element->AddEventListener(Rml::EventId::Click, this);

		if(element->HasAttribute("selected"))
			set_selection(element, true);

		selection_dirty = true;
		box_layout_dirty = true;
	}

	void c_widget_multi_dropdown::on_child_remove(Rml::Element* element) {
		if(element->GetParentNode() != selection_element)
			return;

		element->RemoveEventListener(Rml::EventId::Click, this);

		if(element->HasAttribute("selected"))
			set_selection(element, false);

		selection_dirty = true;
		box_layout_dirty = true;
	}
	
	void c_widget_multi_dropdown::ProcessEvent(Rml::Event& event) {
		if(parent_element->IsDisabled())
			return;

		switch(event.GetId()) {
			case Rml::EventId::Click: {
				if(event.GetCurrentElement()->GetParentNode() == selection_element) {
					for(const int& i : std::views::iota(0, selection_element->GetNumChildren())) {
						if(Rml::Element* current_element{ event.GetCurrentElement() }; selection_element->GetChild(i) == current_element && !event.GetCurrentElement()->HasAttribute("disabled")) {
							switch_selection(current_element);
							event.StopPropagation();

							parent_element->Focus();
						}
					}
				} else {
					Rml::Element* element{ event.GetTargetElement() };
					while(element && element != parent_element) {
						if(element == selection_element) return;
						element = element->GetParentNode();
					}

					show_selectbox(selection_element->GetComputedValues().visibility() == Rml::Style::Visibility::Hidden);
				}
			}
			break;
			case Rml::EventId::Focus: {
				if(event.GetTargetElement() == parent_element) {
					value_element->SetPseudoClass("focus", true);
					button_element->SetPseudoClass("focus", true);
				}
			}
			break;
			case Rml::EventId::Blur: {
				if(event.GetTargetElement() == parent_element) {
					show_selectbox(false);
					value_element->SetPseudoClass("focus", false);
					button_element->SetPseudoClass("focus", false);
				}
			}
			break;
			case Rml::EventId::Keydown: {
				Rml::Input::KeyIdentifier key_identifier{ (Rml::Input::KeyIdentifier)event.GetParameter<int>("key_identifier", 0) };
				if(key_identifier == Rml::Input::KI_RETURN || key_identifier == Rml::Input::KI_NUMPADENTER) {
					parent_element->Click();
					event.StopPropagation();
				}
			}
			break;
			case Rml::EventId::Mousescroll: {
				if(event.GetCurrentElement() == selection_element) {
					event.StopPropagation();
					selection_element->ProcessDefaultAction(event);
				}
			}
			break;
			case Rml::EventId::Scroll: {
				if(box_visible) {
					bool scrolls_selection_box{ };
					for(Rml::Element* element{ event.GetTargetElement() }; element; element = element->GetParentNode()) {
						if(element == selection_element) {
							scrolls_selection_box = true;
							break;
						}
					}

					if(!scrolls_selection_box) show_selectbox(false);
				}
			}
			break;
		}
	}
	
	void c_widget_multi_dropdown::on_value_change(const std::string& value) {
		if(!lock_selection) {
			std::vector<std::string> values{ value | std::views::split('&') | std::views::transform([](const auto& range) { return std::string{ range.begin(), range.end() }; }) | std::ranges::to<std::vector>() };
			for(Rml::Element* option : std::views::iota(0, selection_element->GetNumChildren()) | std::views::transform([&](const int& i) { return selection_element->GetChild(i); })) {
				std::string option_value{ option->GetAttribute("value", std::string{ }) };
				set_selection(option, !option_value.empty() && std::ranges::find(values, option_value) != values.end());
			}
		}

		Rml::Dictionary parameters{ };
		parameters["value"] = value;
		parent_element->DispatchEvent(Rml::EventId::Change, parameters);

		value_rml_dirty = true;
	}

	int c_widget_multi_dropdown::add_option(const std::string& rml, const std::string& option_value, const int& before, const bool& select, const bool& selectable) {
		Rml::ElementPtr element{ Rml::Factory::InstanceElement(selection_element, "*", "option", Rml::XMLAttributes{ }) };
		element->SetInnerRML(rml);

		element->SetAttribute("value", option_value);

		if(select) element->SetAttribute("selected", std::string{ });
		if(!selectable) element->SetAttribute("disabled", std::string{ });

		return add_option(std::move(element), before);
	}
	
	int c_widget_multi_dropdown::add_option(Rml::ElementPtr element, const int& before) {
		if(element->GetTagName() != "option") {
			Rml::Log::Message(Rml::Log::LT_WARNING, "A child of '%s' must be of type 'option' but '%s' was given. See element '%s'.", parent_element->GetTagName().c_str(), element->GetTagName().c_str(), parent_element->GetAddress().c_str());
			return -1;
		}

		const int num_children_before{ selection_element->GetNumChildren() };
		int option_index{ };
		if(before < 0 || before >= num_children_before) {
			selection_element->AppendChild(std::move(element));
			option_index = num_children_before;
		} else {
			selection_element->InsertBefore(std::move(element), selection_element->GetChild(before));
			option_index = before;
		}

		return option_index;
	}
	
	void c_widget_multi_dropdown::remove_option(const int& index) {
		if(Rml::Element* element{ selection_element->GetChild(index) })
			selection_element->RemoveChild(element);
	}
	
	void c_widget_multi_dropdown::clear_options() {
		while(Rml::Element* element{ selection_element->GetLastChild() })
			selection_element->RemoveChild(element);
	}
	
	void c_widget_multi_dropdown::switch_selection(Rml::Element* select_option) {
		if(select_option->HasAttribute("selected")) {
			select_option->RemoveAttribute("selected");
			select_option->SetPseudoClass("checked", false);
		} else {
			select_option->SetAttribute("selected", std::string{ });
			select_option->SetPseudoClass("checked", true);
		}

		update_value();
	}
	
	void c_widget_multi_dropdown::set_selection(Rml::Element* select_option, const bool& selection) {
		if(selection && !select_option->HasAttribute("selected")) {
			select_option->SetAttribute("selected", std::string{ });
			select_option->SetPseudoClass("checked", true);
		} else if(!selection && select_option->HasAttribute("selected")) {
			select_option->RemoveAttribute("selected");
			select_option->SetPseudoClass("checked", false);
		}

		update_value();
	}

	void c_widget_multi_dropdown::update_value() {
		lock_selection = true;
		parent_element->SetAttribute("value", get_selections() | std::views::transform([](const Rml::Element* element) { return element->GetAttribute("value", std::string{ }); }) | std::views::join_with('&') | std::ranges::to<std::string>());
		lock_selection = false;

		value_rml_dirty = true;
	}

	void c_widget_multi_dropdown::show_selectbox(const bool& show) {
		if(show) {
			selection_element->SetProperty(Rml::PropertyId::Visibility, Rml::Property{ Rml::Style::Visibility::Visible });
			value_element->SetPseudoClass("checked", true);
			button_element->SetPseudoClass("checked", true);
			box_layout_dirty = true;
			attach_scroll_event();
		} else {
			selection_element->SetProperty(Rml::PropertyId::Visibility, Rml::Property{ Rml::Style::Visibility::Hidden });
			selection_element->RemoveProperty(Rml::PropertyId::Height);
			value_element->SetPseudoClass("checked", false);
			button_element->SetPseudoClass("checked", false);
			detach_scroll_event();
		}

		box_visible = show;
	}

	void c_widget_multi_dropdown::attach_scroll_event() {
		if(Rml::ElementDocument* document{ parent_element->GetOwnerDocument() })
			document->AddEventListener(Rml::EventId::Scroll, this, true);
	}

	void c_widget_multi_dropdown::detach_scroll_event() {
		if(Rml::ElementDocument* document{ parent_element->GetOwnerDocument() })
			document->RemoveEventListener(Rml::EventId::Scroll, this, true);
	}

	void c_element_form_control_multiselect::move_children() {
		while(Element* raw_child{ GetFirstChild() }) {
			Rml::ElementPtr child{ RemoveChild(raw_child) };
			widget->add_option(std::move(child), -1);
		}
	}

	void c_element_form_control_multiselect::OnAttributeChange(const Rml::ElementAttributes& changed_attributes) {
		RMLUI_ASSERT(widget);

		ElementFormControl::OnAttributeChange(changed_attributes);

		if(const auto& it{ changed_attributes.find("value") }; it != changed_attributes.end())
			widget->on_value_change(it->second.Get<std::string>());
	}
}