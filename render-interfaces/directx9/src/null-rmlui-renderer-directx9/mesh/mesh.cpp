#include "mesh.h"

namespace null::rml::directx9 {
    void c_mesh::on_create() {
        if(vertex_declaration) return;

        constexpr D3DVERTEXELEMENT9 elements[]{
                { 0, offsetof(Rml::Vertex, position), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                { 0, offsetof(Rml::Vertex, tex_coord), D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
                { 0, offsetof(Rml::Vertex, colour), D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
                D3DDECL_END()
        };

        if(auto result{ render::directx9::shared.device->CreateVertexDeclaration(elements, &vertex_declaration) }; FAILED(result))
            utils::logger(utils::e_log_type::error, "cant create vertex input layout, return code {}.", result);
    }

    void c_mesh::on_destroy() {
        if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
        if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
        if(vertex_declaration) { vertex_declaration->Release(); vertex_declaration = nullptr; }
    }

    void c_mesh::compile() {
        static int vtx_buffer_size{ 5000 }, idx_buffer_size{ 10000 };
        if(!vertex_buffer || vtx_buffer_size < geometry_buffer.vertex_buffers_size) {
            if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
            vtx_buffer_size = geometry_buffer.vertex_buffers_size + 5000;
            if(auto result{ render::directx9::shared.device->CreateVertexBuffer(vtx_buffer_size * sizeof(Rml::Vertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vertex_buffer, nullptr) }; FAILED(result))
                utils::logger(utils::e_log_type::error, "cant create vertex buffer, return code {}.", result);
        }

        if(!index_buffer || idx_buffer_size < geometry_buffer.index_buffers_size) {
            if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
            idx_buffer_size = geometry_buffer.index_buffers_size + 10000;
            if(auto result{ render::directx9::shared.device->CreateIndexBuffer(idx_buffer_size * sizeof(int), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &index_buffer, nullptr) }; FAILED(result))
                utils::logger(utils::e_log_type::error, "cant create index buffer, return code {}.", result);
        }

        Rml::Vertex* vertex_dst{ };
        int* index_dst{ };
        if(auto result{ vertex_buffer->Lock(0, (UINT)(geometry_buffer.vertex_buffers_size * sizeof(Rml::Vertex)), (void**)&vertex_dst, D3DLOCK_DISCARD) }; FAILED(result))
            utils::logger(utils::e_log_type::error, "cant lock vertex buffer, return code {}.", result);
        if(auto result{ index_buffer->Lock(0, (UINT)(geometry_buffer.index_buffers_size * sizeof(int)), (void**)&index_dst, D3DLOCK_DISCARD) }; FAILED(result))
            utils::logger(utils::e_log_type::error, "cant lock index buffer, return code {}.", result);

        size_t vertex_offset{ };
        for(const std::span<Rml::Vertex>& vertex_buffer : geometry_buffer.vertex_buffers) {
            std::ranges::move(vertex_buffer, (Rml::Vertex*)vertex_dst + vertex_offset);
            vertex_offset += vertex_buffer.size();
        }

        size_t index_offset{ };
        for(const std::span<int>& index_buffer : geometry_buffer.index_buffers) {
            std::ranges::move(index_buffer, (int*)index_dst + index_offset);
            index_offset += index_buffer.size();
        }

        vertex_buffer->Unlock();
        index_buffer->Unlock();
    }

    void c_mesh::set() {
        render::directx9::shared.device->SetStreamSource(0, vertex_buffer, 0, sizeof(Rml::Vertex));
        render::directx9::shared.device->SetIndices(index_buffer);
        render::directx9::shared.device->SetVertexDeclaration(vertex_declaration);
    }
}