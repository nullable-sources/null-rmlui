#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <null-rmlui.h>
#include <backend/renderer/renderer.h>
#include <graphic/commands/clip-command/clip-command.h>
#include <graphic/commands/matrix-command/matrix-command.h>

namespace null::rml {
    void i_render_interface::SetScissorRegion(int x, int y, int width, int height) {
        draw_list.add_command(std::make_unique<render::commands::c_clip>(rect_t<float>{ (float)x, (float)y, (float)x + (float)width, (float)y + (float)height }));
    }
    
    void i_render_interface::EnableScissorRegion(bool enable) {
        draw_list.add_command(instatnce_clip_enable_command(enable));
    }

    void i_render_interface::SetTransform(const Rml::Matrix4f* transform) {
        matrix4x4_t matrix{ render::backend::renderer->get_projection_matrix() };
        if(transform) matrix *= matrix4x4_t{ *(std::array<float, 16>*)transform->data() };
        draw_list.add_command(std::make_unique<render::commands::c_matrix>(matrix));
    }

    void i_render_interface::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation) {
        draw_list.add_command(instance_geometry_command(num_indices, num_vertices, translation, texture));
        renderer::impl::mesh->geometry_buffer.add_vertex_buffer({ vertices, (size_t)num_vertices });
        renderer::impl::mesh->geometry_buffer.add_index_buffer({ indices, (size_t)num_indices });
    }

    bool i_render_interface::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const std::string& source) {
        Rml::FileInterface* file_interface{ Rml::GetFileInterface() };
        Rml::FileHandle file_handle{ file_interface->Open(source) };

        std::vector<uint8_t> file_data(file_interface->Length(file_handle));
        file_interface->Read(file_data.data(), file_data.size(), file_handle);
        file_interface->Close(file_handle);

        std::uint8_t* data{ stbi_load_from_memory(file_data.data(), file_data.size(), &texture_dimensions.x, &texture_dimensions.y, nullptr, 4) };

        GenerateTexture(texture_handle, data, texture_dimensions);

        stbi_image_free(data);

        return true;
    }

    bool i_render_interface::GenerateTexture(Rml::TextureHandle& texture_handle, const byte* source, const Rml::Vector2i& source_dimensions) {
        texture_handle = (Rml::TextureHandle)render::backend::renderer->create_texture({ (float)source_dimensions.x, (float)source_dimensions.y }, (void*)source);
        return true;
    }

    void i_render_interface::ReleaseTexture(Rml::TextureHandle texture) {
        render::backend::renderer->destroy_texture((void*)texture);
    }

    void i_render_interface::initialize() {
        renderer::impl::mesh = instance_mesh();

        renderer::impl::shaders::passthrough_color = instance_passthrough_color_shader();
        renderer::impl::shaders::passthrough_texture = instance_passthrough_texture_shader();
    }

    void i_render_interface::render() {
        renderer::impl::mesh->compile();

        draw_list.handle();
        draw_list.clear();

        renderer::impl::mesh->clear_geometry();
        render::backend::renderer->setup_state();
    }
}