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
		const Rml::Box& box{ element->GetBox() };

		const Rml::Vector2f padding_size{ box.GetSize(Rml::Box::PADDING).Round() };
		const Rml::Vector2f padding_position{ Rml::Math::RoundFloat(box.GetEdge(Rml::Box::BORDER, Rml::Box::Edge::LEFT)), Rml::Math::RoundFloat(box.GetEdge(Rml::Box::BORDER, Rml::Box::Edge::TOP)) };

		const Rml::ComputedValues& computed{ element->GetComputedValues() };
		return (Rml::DecoratorDataHandle)new data_t{
			{ style.color_tl, style.color_tr, style.color_bl, style.color_br },
			{ padding_position, padding_position + padding_size },
			{ computed.border_top_left_radius(), computed.border_top_right_radius(), computed.border_bottom_left_radius(), computed.border_bottom_right_radius() }
		};
	}

	void c_quad_gradient::RenderElement(Rml::Element* element, Rml::DecoratorDataHandle element_data) const {
		data_t* data{ (data_t*)element_data };

		const Rml::ComputedValues& computed{ element->GetComputedValues() };
		render_interface->draw_list.add_command(std::make_unique<renderer::c_restore_command>());
		render_interface->draw_list.add_convex_shape(
			render::path::make_rect(data->box + (vec2_t<float>)element->GetAbsoluteOffset(Rml::Box::BORDER), data->rounding),
			render::filter_brush_t{ }
				.set_color({ 255, 255, 255, int(computed.opacity() * 255.f) })
				.set_filter(render::quad_gradient_filter_t{ }.set_colors(data->colors))
		);
	}
}