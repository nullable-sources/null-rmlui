#include "null-rmlui-renderer-directx11.h"

#include "null-rmlui-renderer-directx11/shaders/color/color.h"
#include "null-rmlui-renderer-directx11/shaders/texture/texture.h"
#include "null-rmlui-renderer-directx11/shaders/bliting/bliting.h"
#include "null-rmlui-renderer-directx11/shaders/color-matrix/color-matrix.h"
#include "null-rmlui-renderer-directx11/shaders/blend-mask/blend-mask.h"
#include "null-rmlui-renderer-directx11/shaders/gradient/gradient.h"
#include "null-rmlui-renderer-directx11/shaders/blur/blur.h"
#include "null-rmlui-renderer-directx11/shaders/drop-shadow/drop-shadow.h"

namespace ntl::rml::directx11 {
    void c_render::set_secondary_texture(void* texture) {
        render::directx11::shared.context->PSSetShaderResources(1, 1, (ID3D11ShaderResourceView**)&texture);
    }

    std::unique_ptr<renderer::i_mesh_pool> c_render::instance_mesh_pool() {
        return std::make_unique<c_mesh_pool>();
    }

    void c_render::initialize() {
        renderer::color_shader = std::make_unique<c_color_shader>();
        renderer::texture_shader = std::make_unique<c_texture_shader>();
        renderer::bliting_shader = std::make_unique<c_bliting_shader>();
        renderer::color_matrix_shader = std::make_unique<c_color_matrix_shader>();
        renderer::blend_mask_shader = std::make_unique<c_blend_mask_shader>();
        renderer::gradient_shader = std::make_unique<c_gradient_shader>();
        renderer::blur_shader = std::make_unique<c_blur_shader>();
        renderer::drop_shadow_shader = std::make_unique<c_drop_shadow_shader>();
        i_render_interface::initialize();
    }
}