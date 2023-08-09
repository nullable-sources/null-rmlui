#include "null-rmlui.h"

namespace null::rml::extensions {
	std::shared_ptr<Rml::Decorator> c_linear_gradient::instancer_t::InstanceDecorator(const std::string& name, const Rml::PropertyDictionary& properties, const Rml::DecoratorInstancerInterface& instancer_interface) {
		style_t style{
			.num_stops{ properties.GetProperty(num_stops)->Get<int>() },
			.colors{
				properties.GetProperty(color1)->Get<Rml::Colourb>(),
				properties.GetProperty(color2)->Get<Rml::Colourb>(),
				properties.GetProperty(color3)->Get<Rml::Colourb>(),
				properties.GetProperty(color4)->Get<Rml::Colourb>(),
				properties.GetProperty(color5)->Get<Rml::Colourb>(),
				properties.GetProperty(color6)->Get<Rml::Colourb>(),
				properties.GetProperty(color7)->Get<Rml::Colourb>(),
				properties.GetProperty(color8)->Get<Rml::Colourb>(),
				properties.GetProperty(color9)->Get<Rml::Colourb>(),
				properties.GetProperty(color10)->Get<Rml::Colourb>(),
				properties.GetProperty(color11)->Get<Rml::Colourb>(),
				properties.GetProperty(color12)->Get<Rml::Colourb>(),
				properties.GetProperty(color13)->Get<Rml::Colourb>(),
				properties.GetProperty(color14)->Get<Rml::Colourb>(),
				properties.GetProperty(color15)->Get<Rml::Colourb>(),
				properties.GetProperty(color16)->Get<Rml::Colourb>()
			},
			.stops{
				properties.GetProperty(stop1)->Get<float>(),
				properties.GetProperty(stop2)->Get<float>(),
				properties.GetProperty(stop3)->Get<float>(),
				properties.GetProperty(stop4)->Get<float>(),
				properties.GetProperty(stop5)->Get<float>(),
				properties.GetProperty(stop6)->Get<float>(),
				properties.GetProperty(stop7)->Get<float>(),
				properties.GetProperty(stop8)->Get<float>(),
				properties.GetProperty(stop9)->Get<float>(),
				properties.GetProperty(stop10)->Get<float>(),
				properties.GetProperty(stop11)->Get<float>(),
				properties.GetProperty(stop12)->Get<float>(),
				properties.GetProperty(stop13)->Get<float>(),
				properties.GetProperty(stop14)->Get<float>(),
				properties.GetProperty(stop15)->Get<float>(),
				properties.GetProperty(stop16)->Get<float>()
			},
		};

		if(const Rml::Property * property{ properties.GetProperty(angle) }) {
			switch(property->unit) {
				case Rml::Property::RAD: { style.angle = property->Get<float>(); } break;
				case Rml::Property::DEG: { style.angle = angle_t<degrees_t>{ property->Get<float>() }; } break;
				default: return nullptr;
			}
		}

		return std::make_shared<c_linear_gradient>(style);
	}

	Rml::DecoratorDataHandle c_linear_gradient::GenerateElementData(Rml::Element* element) const {
		const Rml::Box& box{ element->GetBox() };

		const Rml::Vector2f padding_size{ box.GetSize(Rml::Box::PADDING).Round() };
		const Rml::Vector2f padding_position{ Rml::Math::RoundFloat(box.GetEdge(Rml::Box::BORDER, Rml::Box::Edge::LEFT)), Rml::Math::RoundFloat(box.GetEdge(Rml::Box::BORDER, Rml::Box::Edge::TOP)) };
		const Rml::Vector2f border{ element->GetAbsoluteOffset(Rml::Box::BORDER) };

		const Rml::ComputedValues& computed{ element->GetComputedValues() };
		return (Rml::DecoratorDataHandle)new data_t{
			style.angle,
			std::views::zip(style.colors, style.stops) | std::views::take(style.num_stops) | std::views::transform([](std::tuple<const color_t<int>&, const float&> tuple) { return std::pair{ std::get<const color_t<int>&>(tuple), std::get<const float&>(tuple) * 0.01f }; }) | std::ranges::to<std::vector>(),
			{ padding_position, padding_position + padding_size },
			{ computed.border_top_left_radius(), computed.border_top_right_radius(), computed.border_bottom_left_radius(), computed.border_bottom_right_radius() }
		};
	}

	void c_linear_gradient::RenderElement(Rml::Element* element, Rml::DecoratorDataHandle element_data) const {
		data_t* data{ (data_t*)element_data };

		const Rml::ComputedValues& computed{ element->GetComputedValues() };
		render_interface->draw_list.add_command(std::make_unique<renderer::c_restore_command>());
		render_interface->draw_list.add_convex_shape(
			render::path::make_rect(data->box + (vec2_t<float>)element->GetAbsoluteOffset(Rml::Box::BORDER), data->rounding),
			render::filter_brush_t{ }
				.set_color({ 255, 255, 255, int(computed.opacity() * 255.f) })
				.set_filter(
					render::linear_gradient_filter_t{ }
						.set_angle(data->angle)
						.set_stops(data->stops)
				)
		);
	}
}