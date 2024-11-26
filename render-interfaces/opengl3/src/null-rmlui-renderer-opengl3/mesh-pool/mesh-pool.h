#pragma once
#include <null-render-backend-opengl3.h>
#include <null-rmlui.h>

namespace ntl::rml::opengl3 {
    class c_mesh_pool : public renderer::i_mesh_pool {
    public: using i_mesh_pool::i_mesh_pool;
    public:
        render::opengl3::c_vertex_element<Rml::Vertex, 2, opengl::e_float, false, offsetof(Rml::Vertex, position)> position{ };
        render::opengl3::c_vertex_element<Rml::Vertex, 4, opengl::e_unsigned_byte, true, offsetof(Rml::Vertex, colour)> color{ };
        render::opengl3::c_vertex_element<Rml::Vertex, 2, opengl::e_float, false, offsetof(Rml::Vertex, tex_coord)> uv{ };
        std::uint32_t vao{ }, vbo{ }, ibo{ };

    public:
        bool create(mesh_t& mesh, Rml::Span<const Rml::Vertex>& vertex_buffer, Rml::Span<const int>& index_buffer) override;

    public:
        void create() override;
        void destroy() override;

    public:
        void compile() override { }

        void use() override;
    };
}