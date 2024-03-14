#pragma once
#include <null-sdk.h>
#include <RmlUi/Core/Types.h>

namespace null::rml::renderer {
	class i_filter { public: virtual void render() = 0; };
	class i_geometry_filter { public: virtual void render(Rml::CompiledGeometryHandle geometry_handle, const Rml::Vector2f& translation, Rml::TextureHandle texture) = 0; };

	class i_filter_instancer { public: virtual std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) = 0; };
	class i_geometry_filter_instancer { public: virtual std::unique_ptr<i_geometry_filter> instance_filter(const Rml::Dictionary& parameters) = 0; };
	
	class c_filter_factory {
	private:
		struct default_instancers_t {
		public:
			std::unique_ptr<i_filter_instancer> brightness_filter{ };
			std::unique_ptr<i_filter_instancer> contrast_filter{ };
			std::unique_ptr<i_filter_instancer> invert_filter{ };
			std::unique_ptr<i_filter_instancer> grayscale_filter{ };
			std::unique_ptr<i_filter_instancer> sepia_filter{ };
			std::unique_ptr<i_filter_instancer> hue_rotate_filter{ };
			std::unique_ptr<i_filter_instancer> saturate_filter{ };
			std::unique_ptr<i_filter_instancer> mask_image_filter{ };
			std::unique_ptr<i_filter_instancer> opacity_filter{ };
			std::unique_ptr<i_filter_instancer> blur_filter{ };
			std::unique_ptr<i_filter_instancer> shadow_filter{ };

			std::unique_ptr<i_geometry_filter_instancer> linear_gradient_geometry_filter{ };
			std::unique_ptr<i_geometry_filter_instancer> radial_gradient_geometry_filter{ };
			std::unique_ptr<i_geometry_filter_instancer> conic_gradient_geometry_filter{ };
		} static inline default_instancers{ };

	private:
		static inline std::unordered_map<std::string, i_filter_instancer*> filter_instancers{ };
		static inline std::unordered_map<std::string, i_geometry_filter_instancer*> geometry_filter_instancers{ };

	public:
		static void register_default_instancers();

		static void register_filter_instancer(const std::string& name, i_filter_instancer* instancer) { filter_instancers[name] = instancer; }
		static void register_geometry_filter_instancer(const std::string& name, i_geometry_filter_instancer* instancer) { geometry_filter_instancers[name] = instancer; }

		static i_filter_instancer* get_filter_instancer(const std::string& name);
		static i_geometry_filter_instancer* get_geometry_filter_instancer(const std::string& name);
	};
}