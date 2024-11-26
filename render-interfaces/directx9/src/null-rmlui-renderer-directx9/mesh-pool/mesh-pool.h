#pragma once
#include <null-render-backend-directx9.h>
#include <null-rmlui.h>

namespace ntl::rml::directx9 {
    class c_mesh_pool : public renderer::i_mesh_pool {
    public: using i_mesh_pool::i_mesh_pool;
    public:
        IDirect3DVertexDeclaration9* vertex_declaration{ };
        IDirect3DVertexBuffer9* vtx_buffer{ };
        IDirect3DIndexBuffer9* idx_buffer{ };

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