#include <null-render-backend-directx11/shaders/passthrough/compiled-object.h>
#include "mesh-pool.h"

namespace ntl::rml::directx11 {
    bool c_mesh_pool::create(mesh_t& mesh, Rml::Span<const Rml::Vertex>& vertex_buffer, Rml::Span<const int>& index_buffer) {
        if(!vtx_buffer || !idx_buffer) create();
        if(!i_mesh_pool::create(mesh, vertex_buffer, index_buffer)) return false;

        D3D11_BOX box{ .top = 0, .front = 0, .bottom = 1, .back = 1 };
        box.left = mesh.vertex_offset * sizeof(Rml::Vertex);
        box.right = box.left + vertex_buffer.size() * sizeof(Rml::Vertex);

        render::directx11::shared.context->UpdateSubresource(vtx_buffer, 0, &box, vertex_buffer.data(), 0, 0);

        box.left = mesh.index_offset * sizeof(int);
        box.right = box.left + index_buffer.size() * sizeof(int);
        render::directx11::shared.context->UpdateSubresource(idx_buffer, 0, &box, index_buffer.data(), 0, 0);

        return true;
    }

    void c_mesh_pool::create() {
        if(input_layout) return;

        D3D11_INPUT_ELEMENT_DESC desc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (std::uint32_t)offsetof(Rml::Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (std::uint32_t)offsetof(Rml::Vertex, tex_coord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (std::uint32_t)offsetof(Rml::Vertex, colour), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        if(auto result = render::directx11::shared.device->CreateInputLayout(desc, 3, render::directx11::sources::passthrough_vs().data(), render::directx11::sources::passthrough_vs().size(), &input_layout); FAILED(result))
            sdk::logger(sdk::e_log_type::error, "cant create vertex input layout, return code {}.", result);

        if(!vtx_buffer) {
            D3D11_BUFFER_DESC buffer_desc{
                .ByteWidth = (std::uint32_t)pool_size * sizeof(Rml::Vertex),
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_INDEX_BUFFER,
                .CPUAccessFlags = 0,
                .MiscFlags = 0
            };
            if(auto result = render::directx11::shared.device->CreateBuffer(&buffer_desc, nullptr, &vtx_buffer); FAILED(result)) {
                sdk::logger(sdk::e_log_type::error, "cant create vertex buffer, return code {}.", result);
                return;
            }
        }

        if(!idx_buffer) {
            D3D11_BUFFER_DESC buffer_desc{
                .ByteWidth = (std::uint32_t)pool_size * sizeof(int),
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_INDEX_BUFFER,
                .CPUAccessFlags = 0,
                .MiscFlags = 0
            };

            if(auto result = render::directx11::shared.device->CreateBuffer(&buffer_desc, nullptr, &idx_buffer); FAILED(result)) {
                sdk::logger(sdk::e_log_type::error, "cant create index buffer, return code {}.", result);
                return;
            }
        }
    }

    void c_mesh_pool::destroy() {
        if(idx_buffer) { idx_buffer->Release(); idx_buffer = nullptr; }
        if(vtx_buffer) { vtx_buffer->Release(); vtx_buffer = nullptr; }
        if(input_layout) { input_layout->Release(); input_layout = nullptr; }
    }

    void c_mesh_pool::use() {
        std::uint32_t stride = sizeof(Rml::Vertex);
        std::uint32_t offset{ };
        render::directx11::shared.context->IASetInputLayout(input_layout);
        render::directx11::shared.context->IASetVertexBuffers(0, 1, &vtx_buffer, &stride, &offset);
        render::directx11::shared.context->IASetIndexBuffer(idx_buffer, DXGI_FORMAT_R32_UINT, 0);
        render::directx11::shared.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}