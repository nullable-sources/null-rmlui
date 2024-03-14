#pragma once
#include <null-render/backend/internal/shader.h>

namespace null::rml::renderer {
	class i_texture_shader : public virtual render::backend::i_shader { }; inline std::unique_ptr<i_texture_shader> texture_shader{ };
}