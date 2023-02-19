#pragma once
#include <RmlUi/Core.h>
#include <null-render.h>

#include <compatibility/color.h>

namespace null::rml::extensions::decorators {
	class c_checkmark : public Rml::Decorator {
	public:
		struct instancer_t : public Rml::DecoratorInstancer {
		public:
			Rml::PropertyId color{ RegisterProperty("color", "white").AddParser("color").GetId() };
			Rml::PropertyId thickness{ RegisterProperty("thickness", "2.5px").AddParser("length").GetId() };
			Rml::PropertyId kick_length{ RegisterProperty("kick-length", "5px").AddParser("length").GetId() }, kick_angle{ RegisterProperty("kick-angle", "50deg").AddParser("angle").GetId() };
			Rml::PropertyId stem_length{ RegisterProperty("stem-length", "10px").AddParser("length").GetId() }, stem_angle{ RegisterProperty("stem-angle", "30deg").AddParser("angle").GetId() };

		public:
			instancer_t() {
				RegisterShorthand("kick", "kick-length, kick-angle", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stem", "stem-length, stem-angle", Rml::ShorthandType::FallThrough);

				RegisterShorthand("decorator", "color, thickness, kick-length, kick-angle, stem-length, stem-angle", Rml::ShorthandType::FallThrough);
			}

		public:
			std::shared_ptr<Rml::Decorator> InstanceDecorator(const std::string& name, const Rml::PropertyDictionary& properties, const Rml::DecoratorInstancerInterface& instancer_interface) override;
		} static inline instancer{ };

		static void register_instancer() { Rml::Factory::RegisterDecoratorInstancer("checkmark", &instancer); }

	public:
		struct style_t {
		public:
			color_t<int> color{ };

			float thickness{ };
			vec2_t<Rml::Style::LengthPercentage> padding{ };

			float kick_length{ }, kick_angle{ };
			float stem_length{ }, stem_angle{ };
		} style{ };

		struct data_t {
		public:
			vec2_t<float> kick{ }, stem{ }, start{ };
		};

	public:
		c_checkmark(const style_t& _style) : style{ _style } { }

	public:
		Rml::DecoratorDataHandle GenerateElementData(Rml::Element* element) const override;
		void ReleaseElementData(Rml::DecoratorDataHandle element_data) const override { delete (data_t*)element_data; }

		void RenderElement(Rml::Element* element, Rml::DecoratorDataHandle element_data) const override;
	};
}