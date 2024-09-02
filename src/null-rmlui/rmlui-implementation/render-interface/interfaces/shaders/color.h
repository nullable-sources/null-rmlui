#pragma once
#include <null-render/backend/internal/shader.h>

namespace ntl::rml::renderer {
    class i_color_shader : public virtual render::backend::i_shader { }; inline std::unique_ptr<i_color_shader> color_shader{ };
}