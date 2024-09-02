#pragma once
#include <null-render/backend/internal/shader.h>

namespace ntl::rml::renderer {
    class i_gradient_shader : public virtual render::backend::i_shader {
    public:
        enum class e_function {
            linear, radial, conic,
            repeating_linear, repeating_radial, repeating_conic
        };

        struct constants_t {
        public:
            e_function function{ };
            vec2_t<float> p{ }, v{ };
            std::vector<std::pair<color_t<float>, float>> stops{ };
        };

    public:
        virtual void set_constants(const constants_t& constants) = 0;
    }; inline std::unique_ptr<i_gradient_shader> gradient_shader{ };
}