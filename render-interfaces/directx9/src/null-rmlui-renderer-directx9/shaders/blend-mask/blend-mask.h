#pragma once
#include <null-rmlui/rmlui-implementation/render-interface/interfaces/shaders/blend-mask.h>
#include <null-render-backend-directx9/internal/shader.h>
#include "compiled-object.h"

namespace null::rml::directx9 {
	class c_blend_mask_shader : public renderer::i_blend_mask_shader, public render::directx9::c_shader {
	public:
		c_blend_mask_shader() : c_shader(&blend_mask_shader_object, &render::directx9::passthrough_vertex_shader_object) { }
	};
}