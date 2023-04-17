#include <null-rmlui.h>

namespace null::rml::extensions::elements {
	c_widget_colorpicker_box::c_widget_colorpicker_box(Rml::ElementFormControl* element) : parent_element{ element } {
		picker_content = parent_element->AppendChild(Rml::Factory::InstanceElement(parent_element, "*", "picker-content", { }));
		picker_box = picker_content->AppendChild(Rml::Factory::InstanceElement(picker_content, "*", "picker-box", { }));
		picker_indicator = picker_box->AppendChild(Rml::Factory::InstanceElement(picker_indicator, "*", "picker-indicator", { }), false);

		inline_block_wrapper = parent_element->AppendChild(Rml::Factory::InstanceElement(parent_element, "*", "inline-block-wrapper", { }));
		
		preview_content = inline_block_wrapper->AppendChild(Rml::Factory::InstanceElement(inline_block_wrapper, "*", "preview-content", { }));
		preview_flex_wrapper = preview_content->AppendChild(Rml::Factory::InstanceElement(preview_content, "*", "preview-flex-wrapper", { }));
		preview_circle = preview_flex_wrapper->AppendChild(Rml::Factory::InstanceElement(preview_flex_wrapper, "*", "preview-circle", { }));

		sliders_content = inline_block_wrapper->AppendChild(Rml::Factory::InstanceElement(inline_block_wrapper, "*", "sliders-content", { }));
		sliders_flex_wrapper = sliders_content->AppendChild(Rml::Factory::InstanceElement(sliders_content, "*", "sliders-flex-wrapper", { }));

		hue_slider = sliders_flex_wrapper->AppendChild(Rml::Factory::InstanceElement(sliders_flex_wrapper, "input", "color-slider", {
			{ "name", Rml::Variant{ "hue" } },
			{ "type", Rml::Variant{ "range" } },
			{ "min", Rml::Variant{ "0" } },
			{ "max", Rml::Variant{ "360" } },
			{ "step", Rml::Variant{ "1" } }
			}));

		alpha_slider = sliders_flex_wrapper->AppendChild(Rml::Factory::InstanceElement(sliders_flex_wrapper, "input", "color-slider", {
			{ "name", Rml::Variant{ "alpha" } },
			{ "type", Rml::Variant{ "range" } },
			{ "min", Rml::Variant{ "0" } },
			{ "max", Rml::Variant{ "1" } },
			{ "value", Rml::Variant{ "1" } },
			{ "step", Rml::Variant{ "0.001" } }
			}));

		parent_element->SetProperty(Rml::PropertyId::Display, { Rml::Style::Display::Block });

		picker_content->SetProperty(Rml::PropertyId::BoxSizing, { Rml::Style::BoxSizing::BorderBox });
		picker_content->SetProperty(Rml::PropertyId::Display, { Rml::Style::Display::Block });
		picker_content->SetProperty(Rml::PropertyId::Width, { 100.f, Rml::Property::PERCENT });
		picker_content->SetProperty(Rml::PropertyId::Height, { 70.f, Rml::Property::PERCENT });

		picker_box->SetProperty(Rml::PropertyId::Display, { Rml::Style::Display::Block });
		picker_box->SetProperty(Rml::PropertyId::Width, { 100.f, Rml::Property::PERCENT });
		picker_box->SetProperty(Rml::PropertyId::Height, { 100.f, Rml::Property::PERCENT });
		picker_box->SetProperty(Rml::PropertyId::Drag, { Rml::Style::Drag::Drag });

		picker_indicator->SetProperty(Rml::PropertyId::Width, { 15.f, Rml::Property::PX });
		picker_indicator->SetProperty(Rml::PropertyId::Height, { 15.f, Rml::Property::PX });

		preview_circle->SetProperty(Rml::PropertyId::Width, { 40.f, Rml::Property::PX });
		preview_circle->SetProperty(Rml::PropertyId::Height, { 40.f, Rml::Property::PX });
		for(const auto& border : { Rml::PropertyId::BorderTopLeftRadius, Rml::PropertyId::BorderTopRightRadius, Rml::PropertyId::BorderBottomRightRadius, Rml::PropertyId::BorderBottomLeftRadius })
			preview_circle->SetProperty(border, { 40, Rml::Property::PX });

		inline_block_wrapper->SetProperty(Rml::PropertyId::Display, { Rml::Style::Display::InlineBlock });
		inline_block_wrapper->SetProperty(Rml::PropertyId::VerticalAlign, { Rml::Style::VerticalAlign::Top });
		inline_block_wrapper->SetProperty(Rml::PropertyId::Width, { 100.f, Rml::Property::PERCENT });
		inline_block_wrapper->SetProperty(Rml::PropertyId::Height, { 30.f, Rml::Property::PERCENT });

		preview_content->SetProperty(Rml::PropertyId::Display, { Rml::Style::Display::InlineBlock });
		preview_content->SetProperty(Rml::PropertyId::Width, { 30.f, Rml::Property::PERCENT });
		preview_content->SetProperty(Rml::PropertyId::Height, { 100.f, Rml::Property::PERCENT });

		preview_flex_wrapper->SetProperty(Rml::PropertyId::Display, { Rml::Style::Display::Flex });
		preview_flex_wrapper->SetProperty(Rml::PropertyId::AlignItems, { Rml::Style::JustifyContent::Center });
		preview_flex_wrapper->SetProperty(Rml::PropertyId::JustifyContent, { Rml::Style::JustifyContent::Center });
		preview_flex_wrapper->SetProperty(Rml::PropertyId::Width, { 100.f, Rml::Property::PERCENT });
		preview_flex_wrapper->SetProperty(Rml::PropertyId::Height, { 100.f, Rml::Property::PERCENT });

		sliders_content->SetProperty(Rml::PropertyId::Display, { Rml::Style::Display::InlineBlock });
		sliders_content->SetProperty(Rml::PropertyId::Width, { 70.f, Rml::Property::PERCENT });
		sliders_content->SetProperty(Rml::PropertyId::Height, { 100.f, Rml::Property::PERCENT });

		sliders_flex_wrapper->SetProperty(Rml::PropertyId::Display, { Rml::Style::Display::Flex });
		sliders_flex_wrapper->SetProperty(Rml::PropertyId::AlignItems, { Rml::Style::JustifyContent::Center });
		sliders_flex_wrapper->SetProperty(Rml::PropertyId::JustifyContent, { Rml::Style::JustifyContent::SpaceAround });
		sliders_flex_wrapper->SetProperty(Rml::PropertyId::FlexDirection, { Rml::Style::FlexDirection::Column });
		sliders_flex_wrapper->SetProperty(Rml::PropertyId::BoxSizing, { Rml::Style::BoxSizing::BorderBox });
		sliders_flex_wrapper->SetProperty(Rml::PropertyId::Width, { 100.f, Rml::Property::PERCENT });
		sliders_flex_wrapper->SetProperty(Rml::PropertyId::Height, { 100.f, Rml::Property::PERCENT });
		sliders_flex_wrapper->SetProperty(Rml::PropertyId::PaddingLeft, { 5.f, Rml::Property::PX });
		sliders_flex_wrapper->SetProperty(Rml::PropertyId::PaddingRight, { 5.f, Rml::Property::PX });

		for(Rml::Element* element : { alpha_slider, hue_slider }) {
			element->SetProperty(Rml::PropertyId::Width, { 100.f, Rml::Property::PERCENT });
			element->SetProperty(Rml::PropertyId::Height, { 15.f, Rml::Property::PX });

			//@note:	https://github.com/mikke89/RmlUi/blob/4032c7f1028efdbaae34ef72d2cdaa8260415b21/Source/Core/Elements/WidgetSlider.cpp#L105
			//			i'm too lazy to write a separate search for GetNumChildren(true) and I can't lead to WidgetSlider, so let's just have an index
			Rml::Element* sliderbar{ element->GetChild(1) };
			sliderbar->SetProperty(Rml::PropertyId::Width, { 6.f, Rml::Property::PX });
			sliderbar->SetProperty(Rml::PropertyId::BackgroundColor, { color_t<int>::palette_t::white, Rml::Property::COLOUR });
			sliderbar->SetProperty(Rml::PropertyId::MarginTop, { -1.f, Rml::Property::PX });
			sliderbar->SetProperty(Rml::PropertyId::BorderTopWidth, { 1.f, Rml::Property::PX });
			sliderbar->SetProperty(Rml::PropertyId::BorderBottomWidth, { 1.f, Rml::Property::PX });
			sliderbar->SetProperty(Rml::PropertyId::BorderLeftWidth, { 1.f, Rml::Property::PX });
			sliderbar->SetProperty(Rml::PropertyId::BorderRightWidth, { 1.f, Rml::Property::PX });
		}

		picker_box->AddEventListener(Rml::EventId::Click, this);
		picker_box->AddEventListener(Rml::EventId::Drag, this);

		preview_circle->AddEventListener(Rml::EventId::Click, this);
		preview_circle->AddEventListener(Rml::EventId::Mouseup, this);

		hue_slider->AddEventListener(Rml::EventId::Change, this);
		alpha_slider->AddEventListener(Rml::EventId::Change, this);

		parent_element->AddEventListener(Rml::EventId::Click, this, true);
		parent_element->AddEventListener(Rml::EventId::Keydown, this, true);
	}

	c_widget_colorpicker_box::~c_widget_colorpicker_box() {
		picker_box->RemoveEventListener(Rml::EventId::Click, this);
		picker_box->RemoveEventListener(Rml::EventId::Drag, this);

		preview_circle->RemoveEventListener(Rml::EventId::Click, this);
		preview_circle->RemoveEventListener(Rml::EventId::Mouseup, this);

		hue_slider->RemoveEventListener(Rml::EventId::Change, this);
		alpha_slider->RemoveEventListener(Rml::EventId::Change, this);

		parent_element->RemoveEventListener(Rml::EventId::Click, this, true);
		parent_element->RemoveEventListener(Rml::EventId::Keydown, this, true);
	}

	void c_widget_colorpicker_box::on_update() {
		if(update_picker_and_preview) {
			update_value();

			Rml::ElementUtilities::FormatElement(picker_indicator, picker_box->GetBox().GetSize(Rml::Box::BORDER));

			vec2_t<float> sv_offset{ picker_box->GetBox().GetSize(Rml::Box::BORDER) * Rml::Vector2f{ picker_box->GetAttribute("saturation", 0.f), 1.f - picker_box->GetAttribute("brightness", 0.f) } };
			vec2_t<float> indicator_size{ picker_indicator->GetBox().GetSize(Rml::Box::BORDER) };
			vec2_t<float> box_size{ picker_box->GetBox().GetSize(Rml::Box::BORDER) };
			picker_indicator->SetOffset(math::min(math::max(sv_offset - indicator_size / 2.f, vec2_t{ 0.f }), box_size - indicator_size), picker_box);
			preview_circle->SetProperty(Rml::PropertyId::BackgroundColor, { color_t<int>{ build_color(), 255 }, Rml::Property::COLOUR });

			if(indicator_size != 0 && box_size != 0) update_picker_and_preview = false;
		}
	}

	void c_widget_colorpicker_box::on_render() {
		if(update_picker_and_preview) on_update();

		const Rml::ComputedValues& picker_box_computed{ picker_box->GetComputedValues() };
		std::array<float, 4> picker_box_roundings{
			picker_box_computed.border_top_left_radius(),
			picker_box_computed.border_top_right_radius(),
			picker_box_computed.border_bottom_left_radius(),
			picker_box_computed.border_bottom_right_radius()
		};
		
		color_t<int> sv_box_color{ hsv_color_t{ hue_slider->GetAttribute("value", 0.f) } };
		rect_t<float> sv_box_rect{ vec2_t<float>{ picker_box->GetAbsoluteOffset(Rml::Box::BORDER) }, vec2_t<float>{ picker_box->GetBox().GetSize(Rml::Box::BORDER) }, e_rect_origin::top_left };
		
		std::vector<vec2_t<float>> path{ null::render::path::make_rect(sv_box_rect.min, sv_box_rect.max, picker_box_roundings) };
		render_interface->draw_list.add_convex_shape(
			path,
			null::render::quad_gradient_brush_t{ }
				.set_top_left_color(color_t<int>::palette_t::white)
				.set_top_right_color(sv_box_color)
				.set_bottom_left_color(color_t<int>{ 255, 0 })
				.set_bottom_right_color(color_t<int>{ sv_box_color, 0 })
		);

		render_interface->draw_list.add_convex_shape(
			path,
			null::render::quad_gradient_brush_t{ }
				.set_top_left_color(color_t<int>{ 0 })
				.set_top_right_color(color_t<int>{ 0 })
				.set_bottom_left_color(color_t<int>::palette_t::black)
				.set_bottom_right_color(color_t<int>::palette_t::black)
		);

		vec2_t<float> picker_indicator_radius{ picker_indicator->GetBox().GetSize(Rml::Box::BORDER) / 2.f };
		float content_radius{ std::max(picker_indicator_radius.x, picker_indicator_radius.y) };
		render_interface->draw_list.add_poly_line(
			null::render::path::make_circle(vec2_t<float>{ picker_indicator->GetAbsoluteOffset(Rml::Box::BORDER) } + content_radius, content_radius),
			null::render::stroke_t{ }.set_thickness(1.f),
			null::render::brush_t{ }.set_color({ 45, 200 })
		);

		render_interface->draw_list.add_poly_line(
			null::render::path::make_circle(vec2_t<float>{ picker_indicator->GetAbsoluteOffset(Rml::Box::BORDER) } + content_radius, content_radius - 2.f),
			null::render::stroke_t{ }.set_thickness(1.f),
			null::render::brush_t{ }.set_color({ build_color(), 255 })
		);

		const Rml::ComputedValues& hue_slider_computed{ hue_slider->GetComputedValues() };
		rect_t<float> hue_slider_rect{ vec2_t<float>{ hue_slider->GetAbsoluteOffset(Rml::Box::CONTENT) }, vec2_t<float>{ hue_slider->GetBox().GetSize(Rml::Box::CONTENT) }, e_rect_origin::top_left };
		vec2_t<float> segment_size{ std::round(hue_slider_rect.size().x / 6.f), hue_slider_rect.size().y };
		for(const int& segment : std::views::iota(0, 6)) {
			color_t<int> hsv{ hsv_color_t{ segment * 60.f } }, next_hsv{ hsv_color_t{ (segment + 1) * 60.f } };
			std::vector<vec2_t<float>> path{ };
			null::render::quad_gradient_brush_t brush{ };
			if(segment == 0) {
				path = null::render::path::make_rect(hue_slider_rect.min, hue_slider_rect.min + math::round(segment_size), { hue_slider_computed.border_top_left_radius(), hue_slider_computed.border_bottom_left_radius(), render::path::e_corner_flags::left });
			} else if(segment == 5) {
				path = null::render::path::make_rect(rect_t{ hue_slider_rect.max, math::round(segment_size), e_rect_origin::bottom_right }, { hue_slider_computed.border_top_right_radius(), hue_slider_computed.border_bottom_right_radius(), render::path::e_corner_flags::right });
			} else path = null::render::path::make_rect(rect_t{ hue_slider_rect.min + vec2_t{ segment_size.x * segment, 0.f }, segment_size, e_rect_origin::top_left });

			render_interface->draw_list.add_convex_shape(
				path,
				null::render::quad_gradient_brush_t{ }
					.set_top_left_color(hsv)
					.set_top_right_color(next_hsv)
					.set_bottom_left_color(hsv)
					.set_bottom_right_color(next_hsv)
			);
		}

		if(alpha_slider->IsVisible()) {
			rect_t<float> alpha_slider_rect{ vec2_t<float>{ alpha_slider->GetAbsoluteOffset(Rml::Box::CONTENT) }, vec2_t<float>{ alpha_slider->GetBox().GetSize(Rml::Box::CONTENT) }, e_rect_origin::top_left };
			const Rml::ComputedValues& alpha_slider_computed{ picker_box->GetComputedValues() };
			std::array<float, 4> alpha_slider_roundings{
				alpha_slider_computed.border_top_left_radius(),
				alpha_slider_computed.border_top_right_radius(),
				alpha_slider_computed.border_bottom_left_radius(),
				alpha_slider_computed.border_bottom_right_radius()
			};
			render_interface->draw_list.add_convex_shape(
				render::path::make_rect(alpha_slider_rect, render::path::rounding_t{ alpha_slider_roundings }),
				null::render::quad_gradient_brush_t{ }
					.set_top_left_color(color_t<int>::palette_t::white)
					.set_top_right_color(color_t<int>{ build_color(), 255 })
					.set_bottom_left_color(color_t<int>::palette_t::white)
					.set_bottom_right_color(color_t<int>{ build_color(), 255 })
			);
		}
	}
	
	void c_widget_colorpicker_box::ProcessEvent(Rml::Event& event) {
		if(parent_element->IsDisabled())
			return;

		switch(event.GetId()) {
			case Rml::EventId::Drag:
			case Rml::EventId::Click: {
				if(event.GetCurrentElement() == picker_box) {
					Rml::Vector2f offset{ (Rml::Vector2f{ event.GetParameter("mouse_x", 0.f), event.GetParameter("mouse_y", 0.f) } - picker_box->GetAbsoluteOffset(Rml::Box::BORDER)) / picker_box->GetBox().GetSize(Rml::Box::BORDER) };
					picker_box->SetAttribute("saturation", std::clamp(offset.x, 0.f, 1.f));
					picker_box->SetAttribute("brightness", std::clamp(1.f - offset.y, 0.f, 1.f));
					update_picker_and_preview = true;
				} else if(event.GetCurrentElement() == preview_circle && event.GetParameter("button", -1) == 0) {
					update_value();
					system_interface->SetClipboardText(parent_element->GetValue());
				}
			} break;

			case Rml::EventId::Mouseup: {
				if(event.GetCurrentElement() == preview_circle && event.GetParameter("button", -1) == 1) {
					std::string clipboard{ };
					system_interface->GetClipboardText(clipboard);
					if(!clipboard.empty() && ((clipboard.starts_with("#") && 7 >= clipboard.size() <= 9) || std::ranges::count(clipboard, ';') == 3)) {
						parent_element->SetAttribute("value", clipboard);
					}
				}
			}

			case Rml::EventId::Change: { update_picker_and_preview = true; } break;
		}
	}
	
	void c_widget_colorpicker_box::on_value_change(const std::string& value) {
		if(!lock_value) {
			std::vector<int> color{ };
			if(value.starts_with("#") && 7 >= value.size() <= 9) {
				color = value | std::views::drop(1) | std::views::chunk(2) | std::views::transform([](const auto& range) { return (int)std::strtol(std::string{ range.begin(), range.end() }.c_str(), nullptr, 16); }) | std::ranges::to<std::vector>();
			} else if(std::ranges::count(value, ';') == 3) {
				color = std::views::split(value, ';') | std::views::transform([](const auto& range) { return std::stoi(std::string{ range.begin(), range.end() }); }) | std::ranges::to<std::vector>();
			}

			hsv_color_t hsv{ color_t<float>{ color } };
			hue_slider->SetAttribute("value", hsv.h);
			picker_box->SetAttribute("saturation", hsv.s);
			picker_box->SetAttribute("brightness", hsv.v);
			if(color.size() == 4) alpha_slider->SetAttribute("value", hsv.a);
		}

		parent_element->DispatchEvent(Rml::EventId::Change, { { "value", Rml::Variant{ value }  } });
	}

	void c_widget_colorpicker_box::update_value() {
		lock_value = true;
		parent_element->SetAttribute("value", build_color().channels | std::views::take(alpha_slider_visible() ? 4 : 3) | std::views::transform([](const int& channel) { return std::to_string(channel); }) | std::views::join_with(';') | std::ranges::to<std::string>());
		lock_value = false;
	}

	void c_element_form_control_colorpicker_box::OnAttributeChange(const Rml::ElementAttributes& changed_attributes) {
		RMLUI_ASSERT(widget);

		ElementFormControl::OnAttributeChange(changed_attributes);

		if(const auto& it{ changed_attributes.find("value") }; it != changed_attributes.end())
			widget->on_value_change(it->second.Get<std::string>());
	}
}