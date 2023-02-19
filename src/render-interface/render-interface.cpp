#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <null-rmlui.h>

namespace null::rml {
    void i_render_interface::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, const Rml::TextureHandle texture, const Rml::Vector2f& translation) {
        buffer.cmd_buffer.back().texture = (void*)texture;

        buffer.add_idx(std::vector<std::uint32_t>{ indices, indices + num_indices }, buffer.vtx_buffer.size());

        buffer.add_vtx(std::views::transform(std::vector<Rml::Vertex>{ vertices, vertices + num_vertices }, [&](const Rml::Vertex& vertex) {
            return null::render::vertex_t{ vertex.position + translation, vertex.tex_coord, vertex.colour };
            }) | std::ranges::to<std::vector>());

        buffer.add_cmd();
    }

    void i_render_interface::EnableScissorRegion(bool enable) {
        buffer.cmd_buffer.back().callbacks.at<null::render::e_cmd_callbacks::on_draw>().set([this, enable](null::render::c_geometry_buffer::cmd_t&) {
            enable_scissor_region(enable);
            return false;
            });
        buffer.add_cmd();
    }

    void i_render_interface::SetScissorRegion(int x, int y, int width, int height) {
        buffer.push_clip_rect(rect_t{ vec2_t{ x, y }, vec2_t{ width, height }, null::e_rect_origin::top_left });
    }

    bool i_render_interface::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const std::string& source) {
        Rml::FileInterface* file_interface{ Rml::GetFileInterface() };
        Rml::FileHandle file_handle{ file_interface->Open(source) };

        std::vector<uint8_t> file_data(file_interface->Length(file_handle));
        file_interface->Read(file_data.data(), file_data.size(), file_handle);
        file_interface->Close(file_handle);

        std::uint8_t* data{ stbi_load_from_memory(file_data.data(), file_data.size(), &texture_dimensions.x, &texture_dimensions.y, nullptr, 4) };

        if(!GenerateTexture(texture_handle, data, texture_dimensions))
            throw std::runtime_error{ "cant generate texture from file" };

        stbi_image_free(data);

        return true;
    }

    void i_render_interface::SetTransform(const Rml::Matrix4f* transform) {
        buffer.cmd_buffer.back().callbacks.at<null::render::e_cmd_callbacks::on_draw>().set([this, transform](null::render::c_geometry_buffer::cmd_t&) {
            set_transform(transform ? matrix4x4_t{ *(std::array<float, 16>*)transform->data() } : matrix4x4_t::identity());
            return false;
            });
        buffer.add_cmd();
    }
}