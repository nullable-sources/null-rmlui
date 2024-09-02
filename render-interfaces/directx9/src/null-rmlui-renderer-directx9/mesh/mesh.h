#pragma once
#include <null-render-backend-directx9.h>
#include <null-rmlui.h>

namespace ntl::rml::directx9 {
    class c_mesh : public renderer::i_mesh {
    public: using i_mesh::i_mesh;
    public:
        IDirect3DVertexDeclaration9* vertex_declaration{ };
        IDirect3DVertexBuffer9* vtx_buffer{ };
        IDirect3DIndexBuffer9* idx_buffer{ };

    public:
        void reassign(std::span<const Rml::Vertex>& _vertex_buffer, std::span<const int>& _index_buffer) override;

    public:
        void create() override;
        void destroy() override;

    public:
        void compile() override;

        void use() override;
    };
}