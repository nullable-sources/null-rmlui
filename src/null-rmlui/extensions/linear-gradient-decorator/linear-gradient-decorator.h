#pragma once
#include <RmlUi/Core.h>

namespace null::rml::extensions {
	class c_linear_gradient : public Rml::Decorator {
	public:
		struct instancer_t : public Rml::DecoratorInstancer {
		public:
			Rml::PropertyId angle{ RegisterProperty("angle", "").AddParser("angle").GetId() };
			Rml::PropertyId num_stops{ RegisterProperty("num_stops", "").AddParser("number").GetId() };
			Rml::PropertyId color1{ RegisterProperty("color[1]", "").AddParser("color").GetId() }; Rml::PropertyId stop1{ RegisterProperty("stop[1]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color2{ RegisterProperty("color[2]", "").AddParser("color").GetId() }; Rml::PropertyId stop2{ RegisterProperty("stop[2]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color3{ RegisterProperty("color[3]", "").AddParser("color").GetId() }; Rml::PropertyId stop3{ RegisterProperty("stop[3]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color4{ RegisterProperty("color[4]", "").AddParser("color").GetId() }; Rml::PropertyId stop4{ RegisterProperty("stop[4]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color5{ RegisterProperty("color[5]", "").AddParser("color").GetId() }; Rml::PropertyId stop5{ RegisterProperty("stop[5]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color6{ RegisterProperty("color[6]", "").AddParser("color").GetId() }; Rml::PropertyId stop6{ RegisterProperty("stop[6]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color7{ RegisterProperty("color[7]", "").AddParser("color").GetId() }; Rml::PropertyId stop7{ RegisterProperty("stop[7]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color8{ RegisterProperty("color[8]", "").AddParser("color").GetId() }; Rml::PropertyId stop8{ RegisterProperty("stop[8]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color9{ RegisterProperty("color[9]", "").AddParser("color").GetId() }; Rml::PropertyId stop9{ RegisterProperty("stop[9]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color10{ RegisterProperty("color[10]", "").AddParser("color").GetId() }; Rml::PropertyId stop10{ RegisterProperty("stop[10]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color11{ RegisterProperty("color[11]", "").AddParser("color").GetId() }; Rml::PropertyId stop11{ RegisterProperty("stop[11]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color12{ RegisterProperty("color[12]", "").AddParser("color").GetId() }; Rml::PropertyId stop12{ RegisterProperty("stop[12]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color13{ RegisterProperty("color[13]", "").AddParser("color").GetId() }; Rml::PropertyId stop13{ RegisterProperty("stop[13]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color14{ RegisterProperty("color[14]", "").AddParser("color").GetId() }; Rml::PropertyId stop14{ RegisterProperty("stop[14]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color15{ RegisterProperty("color[15]", "").AddParser("color").GetId() }; Rml::PropertyId stop15{ RegisterProperty("stop[15]", "").AddParser("length_percent").GetId() };
			Rml::PropertyId color16{ RegisterProperty("color[16]", "").AddParser("color").GetId() }; Rml::PropertyId stop16{ RegisterProperty("stop[16]", "").AddParser("length_percent").GetId() };

		public:
			instancer_t() {
				RegisterShorthand("stops[1]", "color[1], stop[1]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[2]", "color[2], stop[2]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[3]", "color[3], stop[3]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[4]", "color[4], stop[4]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[5]", "color[5], stop[5]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[6]", "color[6], stop[6]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[7]", "color[7], stop[7]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[8]", "color[8], stop[8]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[9]", "color[9], stop[9]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[10]", "color[10], stop[10]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[11]", "color[11], stop[11]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[12]", "color[12], stop[12]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[13]", "color[13], stop[13]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[14]", "color[14], stop[14]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[15]", "color[15], stop[15]", Rml::ShorthandType::FallThrough);
				RegisterShorthand("stops[16]", "color[16], stop[16]", Rml::ShorthandType::FallThrough);

				RegisterShorthand("decorator", "angle, num_stops, color[1], stop[1], color[2], stop[2], color[3], stop[3], color[4], stop[4], color[5], stop[5], color[6], stop[6], color[7], stop[7], color[8], stop[8], color[9], stop[9], color[10], stop[10], color[11], stop[11], color[12], stop[12], color[13], stop[13], color[14], stop[14], color[15], stop[15], color[16], stop[16]", Rml::ShorthandType::FallThrough);
			}

		public:
			std::shared_ptr<Rml::Decorator> InstanceDecorator(const std::string& name, const Rml::PropertyDictionary& properties, const Rml::DecoratorInstancerInterface& instancer_interface) override;
		}; static inline std::unique_ptr<instancer_t> instancer{ };

		static void register_instancer() { instancer = std::make_unique<instancer_t>(); Rml::Factory::RegisterDecoratorInstancer("linear-gradient", instancer.get()); }

	public:
		struct style_t {
		public:
			radians_t angle{ };
			int num_stops{ };

			//@note: https://github.com/mikke89/RmlUi/blob/a29b7fe79ff34309169edd1071c35c68df7d9192/Source/Core/DecoratorGradient.cpp#L411
			std::array<color_t<int>, 16> colors{ };
			std::array<float, 16> stops{ };
		} style{ };

		struct data_t {
		public:
			radians_t angle{ };
			std::vector<std::pair<color_t<int>, float>> stops{ };

			rect_t<float> box{ };
			render::path::rounding_t rounding{ };
		};

	public:
		c_linear_gradient(const style_t& _style) : style{ _style } { }

	public:
		Rml::DecoratorDataHandle GenerateElementData(Rml::Element* element) const override;
		void ReleaseElementData(Rml::DecoratorDataHandle element_data) const override { delete (data_t*)element_data; }

		void RenderElement(Rml::Element* element, Rml::DecoratorDataHandle element_data) const override;
	};
}