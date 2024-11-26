#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <null-render.h>

#include "../../compatibility/rect.h"
#include "../../compatibility/vec2.h"
#include "null-rmlui.h"
#include "interfaces/shaders/texture.h"
#include "interfaces/shaders/color.h"
#include "post-processing/layers.h"

namespace ntl::rml {
    void i_render_interface::EnableScissorRegion(bool enable) {
        if(enable) render::backend::state_pipeline->rasterizers.append_last();
        else {
            rasterizer_state_disabled_scissor->use();
            scissor = rect_t<int>();
        }
    }

    void i_render_interface::SetScissorRegion(Rml::Rectanglei region) {
        scissor = region;
        render::backend::renderer->set_clip(rect_t<float>(region.p0.x, region.p0.y, region.p1.x, region.p1.y));
    }

    void i_render_interface::SetTransform(const Rml::Matrix4f* transform) {
        matrix4x4_t matrix = render::backend::renderer->get_projection_matrix();
        if(transform) matrix *= matrix4x4_t(*(std::array<float, 16>*)transform->data());
        render::backend::renderer->update_matrix(matrix);
    }

    Rml::CompiledGeometryHandle i_render_interface::CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) {
        renderer::i_mesh_pool::mesh_t new_mesh{ };

        for(std::unique_ptr<renderer::i_mesh_pool>& pool : mesh_pools) {
            if(pool->create(new_mesh, vertices, indices))
                return Rml::CompiledGeometryHandle(new renderer::i_mesh_pool::mesh_t(new_mesh));
        }

        std::unique_ptr<renderer::i_mesh_pool>& new_pool = mesh_pools.emplace_back(std::move(instance_mesh_pool()));
        if(new_pool->create(new_mesh, vertices, indices))
            return Rml::CompiledGeometryHandle(new renderer::i_mesh_pool::mesh_t(new_mesh));

        RMLUI_ASSERTMSG(false, "failed to create mesh from mesh pools");

        return Rml::CompiledGeometryHandle();
    }

    void i_render_interface::RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture) {
        renderer::i_mesh_pool::mesh_t* mesh = (renderer::i_mesh_pool::mesh_t*)geometry;
        if(!mesh) return;

        if(texture) {
            render::backend::state_pipeline->shaders.push(renderer::texture_shader);
            render::backend::state_pipeline->textures.push((void*)texture);
        } else {
            render::backend::state_pipeline->shaders.push(renderer::color_shader);
        }

        render::backend::state_pipeline->meshes.push(mesh->pool);
        render::backend::renderer->update_translation(*(vec2_t<float>*)&translation);
        render::backend::renderer->draw_geometry(render::backend::e_topology::triangle_list, mesh->vertex_count, mesh->index_count, mesh->vertex_offset, mesh->index_offset);
        render::backend::state_pipeline->meshes.pop();

        if(texture) render::backend::state_pipeline->textures.pop();
        render::backend::state_pipeline->shaders.pop();
    }

    void i_render_interface::ReleaseGeometry(Rml::CompiledGeometryHandle geometry) {
        renderer::i_mesh_pool::mesh_t* mesh = (renderer::i_mesh_pool::mesh_t*)geometry;
        if(!mesh || !mesh->pool) return;

        mesh->pool->free(*mesh);
        delete mesh;
    }

    Rml::TextureHandle i_render_interface::LoadTexture(Rml::Vector2i& texture_dimensions, const std::string& source) {
        Rml::FileInterface* file_interface = Rml::GetFileInterface();
        Rml::FileHandle file_handle = file_interface->Open(source);

        std::vector<uint8_t> file_data(file_interface->Length(file_handle));
        file_interface->Read(file_data.data(), file_data.size(), file_handle);
        file_interface->Close(file_handle);

        std::uint8_t* data = stbi_load_from_memory(file_data.data(), file_data.size(), &texture_dimensions.x, &texture_dimensions.y, nullptr, 4);
        std::unique_ptr<std::uint8_t[]> premultiplied_data = render::backend::renderer->premultiply_texture_color((vec2_t<int>)texture_dimensions, data);
        Rml::TextureHandle texture_handle = GenerateTexture({ premultiplied_data.get(), (size_t)(texture_dimensions.x * texture_dimensions.y * 4) }, texture_dimensions);
        stbi_image_free(data);

        return texture_handle;
    }

    Rml::TextureHandle i_render_interface::GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions) {
        return (Rml::TextureHandle)render::backend::renderer->create_texture(vec2_t<float>(source_dimensions.x, source_dimensions.y), (void*)source.data());
    }

    void i_render_interface::ReleaseTexture(Rml::TextureHandle texture) {
        render::backend::renderer->destroy_texture((void*)texture);
    }

    void i_render_interface::EnableClipMask(bool enable) {
        render::backend::stencil_buffer->set_test(enable);
    }

    void i_render_interface::RenderToClipMask(Rml::ClipMaskOperation operation, Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation) {
        if(operation == Rml::ClipMaskOperation::Set || operation == Rml::ClipMaskOperation::SetInverse) {
            render::backend::stencil_buffer->clear();
        }

        render::backend::stencil_buffer->set_operation((render::backend::e_stencil_operation)operation);
        RenderGeometry(geometry, translation, { });
        render::backend::stencil_buffer->set_operation(render::backend::e_stencil_operation::end_write);
    }

    Rml::LayerHandle i_render_interface::PushLayer() {
        const Rml::LayerHandle layer_handle = renderer::layers->push();

        render::backend::i_frame_buffer* framebuffer = renderer::layers->layer_by_handle(layer_handle);
        render::backend::state_pipeline->framebuffers.push(framebuffer);
        framebuffer->clear();

        return layer_handle;
    }

    void i_render_interface::CompositeLayers(Rml::LayerHandle source, Rml::LayerHandle destination, Rml::BlendMode blend_mode, Rml::Span<const Rml::CompiledFilterHandle> filters) {
        renderer::layers->primary()->copy_from(renderer::layers->layer_by_handle(source));

        //@note: opengl does not require setting the standard stencil state,
        //       but for dx9/dx11 it is necessary, otherwise no filters will work
        const bool restore_stencil_state = render::backend::stencil_buffer->is_testing() && !filters.empty();
        if(restore_stencil_state) ntl::render::backend::state_pipeline->stencils.push(ntl::render::backend::default_stencil_state);
        for(const Rml::CompiledFilterHandle filter_handle : filters) {
            const compiled_filter_t& filter = *(const compiled_filter_t*)(filter_handle);
            filter.filter->render();
        }
        if(restore_stencil_state) ntl::render::backend::state_pipeline->stencils.pop();

        if(blend_mode == Rml::BlendMode::Replace)
            render::backend::state_pipeline->blends.push(render_interface->blend_state_disabled);

        render::backend::state_pipeline->framebuffers.push(renderer::layers->layer_by_handle(destination));
        render::backend::post_processing->blit_buffer(renderer::layers->primary());
        render::backend::state_pipeline->framebuffers.pop();

        if(destination != renderer::layers->top_layer_handle()) {
            render::backend::state_pipeline->framebuffers.pop();
            render::backend::state_pipeline->framebuffers.push(renderer::layers->top());
        }

        if(blend_mode == Rml::BlendMode::Replace)
            render::backend::state_pipeline->blends.pop();
    }

    void i_render_interface::PopLayer() {
        render::backend::state_pipeline->framebuffers.pop();
        renderer::layers->pop();
    }

    Rml::TextureHandle i_render_interface::SaveLayerAsTexture() {
        Rml::TextureHandle render_texture = GenerateTexture({ }, scissor.size());
        if(!render_texture) return { };

        renderer::layers->blit_top();

        const rect_t<int> initial_scissor = scissor;
        EnableScissorRegion(false);

        rect_t<int> bounds = initial_scissor;
        if(render::backend::renderer->framebuffer_uvs_flipped()) {
            bounds.min.y = render::shared::viewport.y - initial_scissor.min.y;
            bounds.max.y = render::shared::viewport.y - initial_scissor.max.y;
        }

        renderer::layers->secondary()->blit_region_from(renderer::layers->primary(), { }, bounds);
        renderer::layers->secondary()->copy_in_texture((void*)render_texture, rect_t<int>(vec2_t<int>(0.f), initial_scissor.size()));

        SetScissorRegion(initial_scissor);

        return render_texture;
    }

    Rml::CompiledFilterHandle i_render_interface::SaveLayerAsMaskImage() {
        renderer::layers->blit_top();

        render::backend::state_pipeline->blends.push(render_interface->blend_state_disabled);
        render::backend::state_pipeline->framebuffers.push(renderer::layers->blend_mask());
        render::backend::post_processing->blit_buffer(renderer::layers->primary());
        render::backend::state_pipeline->framebuffers.pop();
        render::backend::state_pipeline->blends.pop();

        renderer::i_filter_instancer* instancer = renderer::c_filter_factory::get_filter_instancer("mask-image");
        if(!instancer) {
            Rml::Log::Message(Rml::Log::LT_WARNING, "mask-image filter not registered.");
            return { };
        }

        return (Rml::CompiledFilterHandle)new compiled_filter_t(instancer->instance_filter({ }));
    }

    Rml::CompiledFilterHandle i_render_interface::CompileFilter(const Rml::String& name, const Rml::Dictionary& parameters) {
        renderer::i_filter_instancer* instancer = renderer::c_filter_factory::get_filter_instancer(name);
        if(!instancer) {
            Rml::Log::Message(Rml::Log::LT_WARNING, "Unsupported filter type '%s'.", name.c_str());
            return { };
        }

        return (Rml::CompiledFilterHandle)new compiled_filter_t(instancer->instance_filter(parameters));
    }

    void i_render_interface::ReleaseFilter(Rml::CompiledFilterHandle filter) {
        delete (compiled_filter_t*)(filter);
    }

    Rml::CompiledShaderHandle i_render_interface::CompileShader(const Rml::String& name, const Rml::Dictionary& parameters) {
        std::string fixed_name = name == "shader" ? Rml::Get(parameters, "value", Rml::String()) : name;
        renderer::i_geometry_filter_instancer* instancer = renderer::c_filter_factory::get_geometry_filter_instancer(fixed_name);
        if(!instancer) {
            Rml::Log::Message(Rml::Log::LT_WARNING, "'%s' geometry filter not registered.", fixed_name);
            return { };
        }

        return (Rml::CompiledFilterHandle)new compiled_geometry_filter_t(instancer->instance_filter(parameters));
    }

    void i_render_interface::RenderShader(Rml::CompiledShaderHandle shader_handle, Rml::CompiledGeometryHandle geometry_handle, Rml::Vector2f translation, Rml::TextureHandle texture) {
        const compiled_geometry_filter_t& filter = *(const compiled_geometry_filter_t*)(shader_handle);
        filter.filter->render(geometry_handle, translation, texture);
    }

    void i_render_interface::ReleaseShader(Rml::CompiledShaderHandle shader_handle) {
        delete reinterpret_cast<compiled_geometry_filter_t*>(shader_handle);
    }

    void i_render_interface::initialize() {
        rasterizer_state_disabled_scissor = render::backend::factory->instance_rasterizer_state();
        rasterizer_state_disabled_scissor->unlock();
        rasterizer_state_disabled_scissor->scissor_disable.set(true);
        rasterizer_state_disabled_scissor->lock();

        blend_state_disabled = render::backend::factory->instance_blend_state();
        blend_state_disabled->unlock();
        blend_state_disabled->blend_enable.set(false);
        blend_state_disabled->lock();

        blend_state_factor = render::backend::factory->instance_blend_state();
        blend_state_factor->unlock();
        blend_state_factor->src_blend.set(render::backend::e_blend::constant_factor);
        blend_state_factor->src_alpha_blend.set(render::backend::e_blend::constant_factor);
        blend_state_factor->dst_blend.set(render::backend::e_blend::zero);
        blend_state_factor->dst_alpha_blend.set(render::backend::e_blend::zero);
        blend_state_factor->lock();
    }

    void i_render_interface::begin_render() {
        renderer::layers->push();
        render::backend::state_pipeline->framebuffers.push(renderer::layers->top());
        renderer::layers->top()->clear();
    }

    void i_render_interface::end_render() {
        render::backend::state_pipeline->framebuffers.pop();
        renderer::layers->blit_top();
        render::backend::post_processing->blit_buffer(renderer::layers->primary());

        renderer::layers->pop();

        scissor = rect_t<int>();
    }
}
