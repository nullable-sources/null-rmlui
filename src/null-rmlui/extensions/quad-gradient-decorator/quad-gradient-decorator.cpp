#include "null-rmlui.h"

namespace null::rml::extensions {
	std::shared_ptr<Rml::Decorator> c_quad_gradient::instancer_t::InstanceDecorator(const std::string& name, const Rml::PropertyDictionary& properties, const Rml::DecoratorInstancerInterface& instancer_interface) {
		style_t style{
			.color_tl{ properties.GetProperty(color_tl)->Get<Rml::Colourb>() },
			.color_tr{ properties.GetProperty(color_tr)->Get<Rml::Colourb>() },
			.color_bl{ properties.GetProperty(color_bl)->Get<Rml::Colourb>() },
			.color_br{ properties.GetProperty(color_br)->Get<Rml::Colourb>() },
		};

		return std::make_shared<c_quad_gradient>(style);
	}

	Rml::DecoratorDataHandle c_quad_gradient::GenerateElementData(Rml::Element* element) const {
		const Rml::Box& box = element->GetBox();

		const Rml::Vector2f padding_size = box.GetSize(Rml::BoxArea::Padding).Round();
		const Rml::Vector2f padding_position(Rml::Math::Round(box.GetEdge(Rml::BoxArea::Border, Rml::BoxEdge::Left)), Rml::Math::Round(box.GetEdge(Rml::BoxArea::Border, Rml::BoxEdge::Top)));

		const Rml::ComputedValues& computed{ element->GetComputedValues() };
		std::shared_ptr<render::c_quad_gradient_filter> filter = render::c_quad_gradient_filter::instance();
		filter->set_colors({ style.color_tl, style.color_tr, style.color_bl, style.color_br });

		std::shared_ptr<render::c_filter_brush> brush = render::c_filter_brush::instance();
		brush->set_filter(filter);

		std::shared_ptr<render::c_draw_list> draw_list = render::c_draw_list::instance(render::backend::factory->instance_mesh());
		draw_list->add_convex_shape(
			render::make_rect(rect_t<float>(padding_position, padding_position + padding_size), { computed.border_top_left_radius(), computed.border_top_right_radius(), computed.border_bottom_left_radius(), computed.border_bottom_right_radius() }),
			brush
		);

		draw_list->compile();

		return (Rml::DecoratorDataHandle)new data_t(std::move(draw_list));
	}

	void c_quad_gradient::RenderElement(Rml::Element* element, Rml::DecoratorDataHandle element_data) const {
		data_t* data = (data_t*)element_data;

		render_interface->command_buffer.add_command(render::c_update_translation_command::instance(element->GetAbsoluteOffset(Rml::BoxArea::Border)));
		render_interface->command_buffer.add_command(data->draw_list);
	}
}