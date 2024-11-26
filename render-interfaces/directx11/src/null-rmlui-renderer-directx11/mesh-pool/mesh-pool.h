#pragma once
#include <null-render-backend-directx11.h>
#include <null-rmlui.h>

namespace ntl::rml::directx11 {
    class c_mesh_pool : public renderer::i_mesh_pool {
    public: using i_mesh_pool::i_mesh_pool;
    public:
        ID3D11InputLayout* input_layout{ };
        ID3D11Buffer* vtx_buffer{ }, *idx_buffer{ };

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