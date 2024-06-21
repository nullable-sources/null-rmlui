#pragma once
#include "bliting.h"

namespace null::rml::renderer {
    class i_drop_shadow_shader : public virtual render::backend::i_shader {
    public:
        struct constants_t : public i_bliting_shader::constants_t {
        public:
            color_t<float> color{ };
        };

    public:
        virtual void set_constants(const constants_t& constants) = 0;
    }; inline std::unique_ptr<i_drop_shadow_shader> drop_shadow_shader{ };
}