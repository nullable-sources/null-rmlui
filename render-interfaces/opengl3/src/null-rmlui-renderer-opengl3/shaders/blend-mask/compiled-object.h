#pragma once
#include <null-render-backend-opengl3/shaders/passthrough/compiled-object.h>

namespace ntl::rml::opengl3 {
    class c_blend_mask_shader_object : public render::opengl3::c_fragment_shader {
    public:
        resource_t get_source() override {
            return resource_t("null-rml:opengl:shaders:blend-mask.frag", "null-rml:opengl:resources");
        }
    } inline blend_mask_shader_object{ };
}