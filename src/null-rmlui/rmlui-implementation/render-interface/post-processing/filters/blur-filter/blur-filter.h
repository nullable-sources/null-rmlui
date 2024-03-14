#pragma once
#include <null-render/backend/internal/frame-buffer.h>
#include "../factory/factory.h"
#include "../../../interfaces/shaders/blur.h"

namespace null::rml::renderer::filters {
	class c_blur_filter : public i_filter {
	private:
		static void calculate_weights(std::vector<float>& weights, float sigma);

	public:
		static void make_blur(float sigma, render::backend::i_frame_buffer* src, render::backend::i_frame_buffer* temp);

	public:
		float sigma{ };

	public:
		c_blur_filter() { }
		c_blur_filter(float _sigma) : sigma(_sigma) { }

	public:
		void render() override;
	};

	class c_blur_filter_instancer : public i_filter_instancer {
	public:
		std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
	};
}