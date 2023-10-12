#pragma once
#include <RmlUi/Core.h>

namespace null::rml::extensions {
	class c_quad_gradient : public Rml::Decorator {
	public:
		struct instancer_t : public Rml::DecoratorInstancer {
		public:
			Rml::PropertyId color_tl{ RegisterProperty("color_tl", "").AddParser("color").GetId() };
			Rml::PropertyId color_tr{ RegisterProperty("color_tr", "").AddParser("color").GetId() };
			Rml::PropertyId color_bl{ RegisterProperty("color_bl", "").AddParser("color").GetId() };
			Rml::PropertyId color_br{ RegisterProperty("color_br", "").AddParser("color").GetId() };

		public:
			instancer_t() {
				RegisterShorthand("top", "color_tl, color_tr", Rml::ShorthandType::FallThrough);
				RegisterShorthand("bottom", "color_bl, color_br", Rml::ShorthandType::FallThrough);
				RegisterShorthand("left", "color_tl, color_bl", Rml::ShorthandType::FallThrough);
				RegisterShorthand("right", "color_tr, color_br", Rml::ShorthandType::FallThrough);

				RegisterShorthand("decorator", "color_tl, color_tr, color_bl, color_br", Rml::ShorthandType::FallThrough);
			}

		public:
			std::shared_ptr<Rml::Decorator> InstanceDecorator(const std::string& name, const Rml::PropertyDictionary& properties, const Rml::DecoratorInstancerInterface& instancer_interface) override;
		}; static inline std::unique_ptr<instancer_t> instancer{ };

		static void register_instancer() { instancer = std::make_unique<instancer_t>(); Rml::Factory::RegisterDecoratorInstancer("quad-gradient", instancer.get()); }

	public:
		struct style_t {
		public:
			color_t<int> color_tl{ }, color_tr{ }, color_bl{ }, color_br{ };
		} style{ };

		struct data_t {
		public:
			std::array<color_t<int>, 4> colors{ };

			rect_t<float> box{ };
			render::path::rounding_t rounding{ };
		};

	public:
		c_quad_gradient(const style_t& _style) : style{ _style } { }

	public:
		Rml::DecoratorDataHandle GenerateElementData(Rml::Element* element) const override;
		void ReleaseElementData(Rml::DecoratorDataHandle element_data) const override { delete (data_t*)element_data; }

		void RenderElement(Rml::Element* element, Rml::DecoratorDataHandle element_data) const override;
	};
}