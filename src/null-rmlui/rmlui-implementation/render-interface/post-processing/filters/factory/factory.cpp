#include "factory.h"
#include "../color-matrix-filter/color-matrix-filter.h"
#include "../mask-image-filter/mask-image-filter.h"
#include "../passthrough-filter/passthrough-filter.h"
#include "../gradient-filter/gradient-shader-filter.h"
#include "../blur-filter/blur-filter.h"
#include "../shadow-filter/shadow-filter.h"

namespace null::rml::renderer {
	void c_filter_factory::register_default_instancers() {
		default_instancers.brightness_filter = std::make_unique<filters::c_brightness_filter_instancer>();
		default_instancers.contrast_filter = std::make_unique<filters::c_contrast_filter_instancer>();
		default_instancers.invert_filter = std::make_unique<filters::c_invert_filter_instancer>();
		default_instancers.grayscale_filter = std::make_unique<filters::c_grayscale_filter_instancer>();
		default_instancers.sepia_filter = std::make_unique<filters::c_sepia_filter_instancer>();
		default_instancers.hue_rotate_filter = std::make_unique<filters::c_hue_rotate_filter_instancer>();
		default_instancers.saturate_filter = std::make_unique<filters::c_saturate_filter_instancer>();
		default_instancers.mask_image_filter = std::make_unique<filters::c_mask_image_filter_instancer>();
		default_instancers.opacity_filter = std::make_unique<filters::c_opacity_filter_instancer>();
		default_instancers.blur_filter = std::make_unique<filters::c_blur_filter_instancer>();
		default_instancers.shadow_filter = std::make_unique<filters::c_shadow_filter_instancer>();

		default_instancers.linear_gradient_geometry_filter = std::make_unique<filters::c_linear_gradient_shader_filter_instancer>();
		default_instancers.radial_gradient_geometry_filter = std::make_unique<filters::c_radial_gradient_shader_filter_instancer>();
		default_instancers.conic_gradient_geometry_filter = std::make_unique<filters::c_conic_gradient_shader_filter_instancer>();

		register_filter_instancer("brightness", default_instancers.brightness_filter.get());
		register_filter_instancer("contrast", default_instancers.contrast_filter.get());
		register_filter_instancer("invert", default_instancers.invert_filter.get());
		register_filter_instancer("grayscale", default_instancers.grayscale_filter.get());
		register_filter_instancer("sepia", default_instancers.sepia_filter.get());
		register_filter_instancer("hue-rotate", default_instancers.hue_rotate_filter.get());
		register_filter_instancer("saturate", default_instancers.saturate_filter.get());
		register_filter_instancer("mask-image", default_instancers.mask_image_filter.get());
		register_filter_instancer("opacity", default_instancers.opacity_filter.get());
		register_filter_instancer("blur", default_instancers.blur_filter.get());
		register_filter_instancer("drop-shadow", default_instancers.shadow_filter.get());

		register_geometry_filter_instancer("linear-gradient", default_instancers.linear_gradient_geometry_filter.get());
		register_geometry_filter_instancer("radial-gradient", default_instancers.radial_gradient_geometry_filter.get());
		register_geometry_filter_instancer("conic-gradient", default_instancers.conic_gradient_geometry_filter.get());
	}

	i_filter_instancer* c_filter_factory::get_filter_instancer(const std::string& name) {
		if(!filter_instancers.contains(name)) return nullptr;
		return filter_instancers[name];
	}

	i_geometry_filter_instancer* c_filter_factory::get_geometry_filter_instancer(const std::string& name) {
		if(!geometry_filter_instancers.contains(name)) return nullptr;
		return geometry_filter_instancers[name];
	}
}