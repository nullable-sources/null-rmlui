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

		if(const Rml::Property* property = properties.GetProperty(angle)) {
			switch(property->unit) {
				case Rml::Unit::RAD: { style.angle = property->Get<float>(); } break;
				case Rml::Unit::DEG: { style.angle = angle_t<degrees_t>(property->Get<float>()); } break;
				default: return nullptr;
			}
		}

		return std::make_shared<c_linear_gradient>(style);
	}

	Rml::DecoratorDataHandle c_linear_gradient::GenerateElementData(Rml::Element* element) const {
		const Rml::Box& box = element->GetBox();

		const Rml::Vector2f padding_size = box.GetSize(Rml::BoxArea::Padding).Round();
		const Rml::Vector2f padding_position(Rml::Math::Round(box.GetEdge(Rml::BoxArea::Border, Rml::BoxEdge::Left)), Rml::Math::Round(box.GetEdge(Rml::BoxArea::Border, Rml::BoxEdge::Top)));

		const Rml::ComputedValues& computed = element->GetComputedValues();

		std::shared_ptr<render::c_linear_gradient_filter> filter = render::c_linear_gradient_filter::instance();
		filter->set_angle(style.angle);
		filter->set_stops(std::views::zip(style.colors, style.stops) | std::views::take(style.num_stops) | std::views::transform([](std::tuple<const color_t<int>&, const float&> tuple) { return std::pair(std::get<const color_t<int>&>(tuple), std::get<const float&>(tuple) * 0.01f); }) | std::ranges::to<std::vector>());

		std::shared_ptr<render::c_filter_brush> brush = render::c_filter_brush::instance();
		brush->set_filter(filter);
		brush->set_color(color_t<int>(255, computed.opacity()));

		std::shared_ptr<render::c_draw_list> draw_list = render::c_draw_list::instance(render::backend::factory->instance_mesh());
		draw_list->add_convex_shape(
			render::make_rect(rect_t<float>(padding_position, padding_position + padding_size), { computed.border_top_left_radius(), computed.border_top_right_radius(), computed.border_bottom_left_radius(), computed.border_bottom_right_radius() }),
			brush
		);

		draw_list->compile();

		return (Rml::DecoratorDataHandle)new data_t(std::move(draw_list));
	}

	void c_linear_gradient::RenderElement(Rml::Element* element, Rml::DecoratorDataHandle element_data) const {
		data_t* data = (data_t*)element_data;

		render_interface->command_buffer.add_command(render::c_update_translation_command::instance(element->GetAbsoluteOffset(Rml::BoxArea::Border)));
		render_interface->command_buffer.add_command(data->draw_list);
	}
}