#include "mesh-pool.h"

namespace ntl::rml::directx9 {
    bool c_mesh_pool::create(mesh_t& mesh, Rml::Span<const Rml::Vertex>& vertex_buffer, Rml::Span<const int>& index_buffer) {
        if(!vertex_declaration) create();
        if(!i_mesh_pool::create(mesh, vertex_buffer, index_buffer)) return false;

        Rml::Vertex* vertex_dst{ };
        int* index_dst{ };
        if(auto result = vtx_buffer->Lock((UINT)(mesh.vertex_offset * sizeof(Rml::Vertex)), (UINT)(vertex_buffer.size() * sizeof(Rml::Vertex)), (void**)&vertex_dst, 0); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant lock vertex buffer, return code {}.", result);
        if(auto result = idx_buffer->Lock((UINT)(mesh.index_offset * sizeof(int)), (UINT)(index_buffer.size() * sizeof(int)), (void**)&index_dst, 0); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant lock index buffer, return code {}.", result);

        std::ranges::move(vertex_buffer, vertex_dst);
        std::ranges::move(index_buffer, index_dst);

        vtx_buffer->Unlock();
        idx_buffer->Unlock();
        return true;
    }

    void c_mesh_pool::create() {
        if(vertex_declaration) return;

        constexpr D3DVERTEXELEMENT9 elements[]{
            { 0, offsetof(Rml::Vertex, position), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
            { 0, offsetof(Rml::Vertex, tex_coord), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
            { 0, offsetof(Rml::Vertex, colour), D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
            D3DDECL_END()
        };

        if(auto result = render::directx9::shared.device->CreateVertexDeclaration(elements, &vertex_declaration); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant create vertex input layout, return code {}.", result);

        if(!vtx_buffer) {
            if(auto result = render::directx9::shared.device->CreateVertexBuffer(pool_size * sizeof(Rml::Vertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vtx_buffer, nullptr); FAILED(result))
                sdk::logger(sdk::e_log_type::error, "cant create vertex buffer, return code {}.", result);
        }

        if(!idx_buffer) {
            if(auto result = render::directx9::shared.device->CreateIndexBuffer(pool_size * sizeof(int), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &idx_buffer, nullptr); FAILED(result))
                sdk::logger(sdk::e_log_type::error, "cant create index buffer, return code {}.", result);
        }
    }

    void c_mesh_pool::destroy() {
        if(idx_buffer) { idx_buffer->Release(); idx_buffer = nullptr; }
        if(vtx_buffer) { vtx_buffer->Release(); vtx_buffer = nullptr; }
        if(vertex_declaration) { vertex_declaration->Release(); vertex_declaration = nullptr; }
    }

    void c_mesh_pool::use() {
        render::directx9::shared.device->SetStreamSource(0, vtx_buffer, 0, sizeof(Rml::Vertex));
        render::directx9::shared.device->SetIndices(idx_buffer);
        render::directx9::shared.device->SetVertexDeclaration(vertex_declaration);
    }
}