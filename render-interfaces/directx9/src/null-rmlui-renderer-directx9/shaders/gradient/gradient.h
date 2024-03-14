#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/gradient.h>
#include <null-render-backend-directx9/internal/shader.h>
#include "../color/compiled-object.h"
#include "compiled-object.h"

namespace null::rml::directx9 {
	class c_gradient_shader : public renderer::i_gradient_shader, public render::directx9::c_shader {
	public:
		c_gradient_shader() : c_shader(&gradient_shader_object, &color_shader_object) { }

	public:
		void set_constants(const constants_t& constants) override;
	};
}