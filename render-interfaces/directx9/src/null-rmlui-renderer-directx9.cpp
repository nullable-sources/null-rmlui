#include "null-rmlui-renderer-directx9.h"
#include "null-rmlui-renderer-directx9/mesh/mesh.h"

#include "null-rmlui-renderer-directx9/shaders/color/color.h"
#include "null-rmlui-renderer-directx9/shaders/texture/texture.h"
#include "null-rmlui-renderer-directx9/shaders/bliting/bliting.h"
#include "null-rmlui-renderer-directx9/shaders/color-matrix/color-matrix.h"
#include "null-rmlui-renderer-directx9/shaders/blend-mask/blend-mask.h"
#include "null-rmlui-renderer-directx9/shaders/gradient/gradient.h"
#include "null-rmlui-renderer-directx9/shaders/blur/blur.h"
#include "null-rmlui-renderer-directx9/shaders/drop-shadow/drop-shadow.h"

namespace ntl::rml::directx9 {
    void c_render::set_secondary_texture(void* texture) {
        render::directx9::shared.device->SetTexture(1, (IDirect3DTexture9*)texture);
    }

    Rml::CompiledGeometryHandle c_render::CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) {
        i_render_interface::compiled_geometry_t* geometry = new i_render_interface::compiled_geometry_t(mesh_pool.push(std::make_unique<c_mesh>()));
        geometry->compile_mesh(vertices, indices);
        return (Rml::CompiledGeometryHandle)geometry;
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