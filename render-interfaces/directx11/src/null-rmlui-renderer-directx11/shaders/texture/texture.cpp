#include "texture.h"

namespace ntl::rml::directx11 {
    void c_texture_shader::use() {
        if(empty()) return;
        c_shader::use();

        color_shader_object.set_constant({ ntl::render::backend::renderer->get_matrix(), ntl::render::backend::renderer->get_translation() });
    }
}