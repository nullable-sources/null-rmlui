#include <rmlui-implementation/render-interface/render-interface.h>
#include <compatibility/vec2.h>
#include <extensions/decorators/checkmark/checkmark.h>

namespace null::rml::extensions::decorators {
	std::shared_ptr<Rml::Decorator> c_checkmark::instancer_t::InstanceDecorator(const std::string& name, const Rml::PropertyDictionary& properties, const Rml::DecoratorInstancerInterface& instancer_interface) {
		c_checkmark::style_t style{
			.color{ properties.GetProperty(color)->Get<Rml::Colourb>() },
			.thickness{ properties.GetProperty(thickness)->Get<float>() },

			.kick_length{ properties.GetProperty(kick_length)->Get<float>() },
			.stem_length{ properties.GetProperty(stem_length)->Get<float>() }
		};

		if(const Rml::Property * property{ properties.GetProperty(kick_angle) }) {
			switch(property->unit) {
				case Rml::Property::DEG: { style.kick_angle = -(radians_t)angle_t<radians_t>{ 90.f + property->Get<float>() }; } break;
				case Rml::Property::RAD: { style.kick_angle = -(radians_t)angle_t<radians_t>{ 90.f } + property->Get<double>(); } break;
				default: return nullptr;
			}
		}

		if(const Rml::Property * property{ properties.GetProperty(stem_angle) }) {
			switch(property->unit) {
				case Rml::Property::DEG: { style.stem_angle = -(radians_t)angle_t<radians_t>{ 90.f - property->Get<float>() }; } break;
				case Rml::Property::RAD: { style.stem_angle = -(radians_t)angle_t<radians_t>{ 90.f } - property->Get<double>(); } break;
				default: return nullptr;
			}
		}

		return std::make_shared<c_checkmark>(style);
	}

	Rml::DecoratorDataHandle c_checkmark::GenerateElementData(Rml::Element* element) const {
		vec2_t kick{ vec2_t<float>{ std::cosf(style.kick_angle), std::sinf(style.kick_angle) } * style.kick_length };
		vec2_t stem{ vec2_t<float>{ std::cosf(style.stem_angle), std::sinf(style.stem_angle) } * style.stem_length };
		vec2_t offset{ element->GetBox().GetSize(Rml::Box::BORDER) / 2.f - (kick + stem) / 3.f };

		return (Rml::DecoratorDataHandle)new data_t{ kick + offset, stem + offset, offset };
	}

	void c_checkmark::RenderElement(Rml::Element* element, Rml::DecoratorDataHandle element_data) const {
		data_t* data{ (data_t*)element_data };
		vec2_t element_position{ element->GetAbsoluteOffset(Rml::Box::BORDER) };

		render_interface->draw_list.add_command(std::make_unique<null::rml::renderer::impl::commands::c_restore>());
		render_interface->draw_list.add_poly_line(
			{ data->kick + element_position, data->start + element_position, data->stem + element_position },
			null::render::stroke_t{ }.set_thickness(style.thickness),
			null::render::brush_t{ }.set_color(style.color)
		);
	}
}