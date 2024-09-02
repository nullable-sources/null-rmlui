#pragma once
#include <null-render/backend/internal/shader.h>

namespace ntl::rml::renderer {
    class i_blend_mask_shader : public virtual render::backend::i_shader { }; inline std::unique_ptr<i_blend_mask_shader> blend_mask_shader{ };
}