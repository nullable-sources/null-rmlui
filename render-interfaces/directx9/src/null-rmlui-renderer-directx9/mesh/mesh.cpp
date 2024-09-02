#include "mesh.h"

namespace ntl::rml::directx9 {
    void c_mesh::reassign(std::span<const Rml::Vertex>& _vertex_buffer, std::span<const int>& _index_buffer) {
        if(_vertex_buffer.size() > vertex_buffer.size()) {
            if(vtx_buffer) { vtx_buffer->Release(); vtx_buffer = nullptr; }
        }

        if(_index_buffer.size() > index_buffer.size()) {
            if(idx_buffer) { idx_buffer->Release(); idx_buffer = nullptr; }
        }

        i_mesh::reassign(_vertex_buffer, _index_buffer);
    }

    void c_mesh::create() {
        if(vertex_declaration) return;

        constexpr D3DVERTEXELEMENT9 elements[]{
            { 0, offsetof(Rml::Vertex, position), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, offsetof(Rml::Vertex, tex_coord), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, offsetof(Rml::Vertex, colour), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            D3DDECL_END()
        };

        if(auto result = render::directx9::shared.device->CreateVertexDeclaration(elements, &vertex_declaration); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant create vertex input layout, return code {}.", result);
    }

    void c_mesh::destroy() {
        if(idx_buffer) { idx_buffer->Release(); idx_buffer = nullptr; }
        if(vtx_buffer) { vtx_buffer->Release(); vtx_buffer = nullptr; }
        if(vertex_declaration) { vertex_declaration->Release(); vertex_declaration = nullptr; }
    }

    void c_mesh::compile() {
        if(!vertex_declaration) create();

        if(!vtx_buffer) {
            if(auto result = render::directx9::shared.device->CreateVertexBuffer(vertex_buffer.size() * sizeof(Rml::Vertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vtx_buffer, nullptr); FAILED(result))
                sdk::logger(sdk::e_log_type::error, "cant create vertex buffer, return code {}.", result);
        }

        if(!idx_buffer) {
            if(auto result = render::directx9::shared.device->CreateIndexBuffer(index_buffer.size() * sizeof(int), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &idx_buffer, nullptr); FAILED(result))
                sdk::logger(sdk::e_log_type::error, "cant create index buffer, return code {}.", result);
        }

        Rml::Vertex* vertex_dst{ };
        int* index_dst{ };
        if(auto result = vtx_buffer->Lock(0, (UINT)(vertex_buffer.size() * sizeof(Rml::Vertex)), (void**)&vertex_dst, D3DLOCK_DISCARD); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant lock vertex buffer, return code {}.", result);
        if(auto result = idx_buffer->Lock(0, (UINT)(index_buffer.size() * sizeof(int)), (void**)&index_dst, D3DLOCK_DISCARD); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant lock index buffer, return code {}.", result);

        std::ranges::move(vertex_buffer, vertex_dst);
        std::ranges::move(index_buffer, index_dst);

        vtx_buffer->Unlock();
        idx_buffer->Unlock();
    }

    void c_mesh::use() {
        render::directx9::shared.device->SetStreamSource(0, vtx_buffer, 0, sizeof(Rml::Vertex));
        render::directx9::shared.device->SetIndices(idx_buffer);
        render::directx9::shared.device->SetVertexDeclaration(vertex_declaration);
    }
}