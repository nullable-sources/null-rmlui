#pragma once
#include "../factory/factory.h"

namespace null::rml::renderer::filters {
	class c_passthrough_filter : public i_filter {
	public:
		float blend_factor{ };

	public:
		c_passthrough_filter() { }
		c_passthrough_filter(float _blend_factor) : blend_factor(_blend_factor) { }

	public:
		void render() override;
	};

	class c_opacity_filter_instancer : public i_filter_instancer {
	public:
		std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
	};
}