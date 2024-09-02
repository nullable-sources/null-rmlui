#pragma once
#include <null-render/backend/internal/shader.h>

namespace ntl::rml::renderer {
    class i_color_matrix_shader : public virtual render::backend::i_shader {
    public:
        struct constants_t {
        public:
            matrix4x4_t color_matrix{ };
        };

    public:
        virtual void set_constants(const constants_t& constants) = 0;
    }; inline std::unique_ptr<i_color_matrix_shader> color_matrix_shader{ };
}