#include <null-render-backend-directx11/shaders/passthrough/compiled-object.h>
#include "mesh.h"

namespace null::rml::directx11 {
	void c_mesh::create() {
		if(input_layout) return;

		D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION",	0, DXGI_FORMAT_R32G32_FLOAT,	0, (std::uint32_t)offsetof(Rml::Vertex, position),	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, (std::uint32_t)offsetof(Rml::Vertex, tex_coord),	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, (std::uint32_t)offsetof(Rml::Vertex, colour),	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		if(auto result = render::directx11::shared.device->CreateInputLayout(desc, 3, render::directx11::sources::passthrough_vs().data(), render::directx11::sources::passthrough_vs().size(), &input_layout); FAILED(result))
			utils::logger(utils::e_log_type::error, "cant create vertex input layout, return code {}.", result);
	}

	void c_mesh::destroy() {
		if(idx_buffer) { idx_buffer->Release(); idx_buffer = nullptr; }
		if(vtx_buffer) { vtx_buffer->Release(); vtx_buffer = nullptr; }
		if(input_layout) { input_layout->Release(); input_layout = nullptr; }
	}

	void c_mesh::compile() {
		if(!vtx_buffer) {
			D3D11_BUFFER_DESC buffer_desc{
				.ByteWidth = (std::uint32_t)vertex_buffer.size() * sizeof(Rml::Vertex),
				.Usage = D3D11_USAGE_DYNAMIC,
				.BindFlags = D3D11_BIND_VERTEX_BUFFER,
				.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
				.MiscFlags = 0
			};
			if(auto result = render::directx11::shared.device->CreateBuffer(&buffer_desc, nullptr, &vtx_buffer); FAILED(result)) {
				utils::logger(utils::e_log_type::error, "cant create vertex buffer, return code {}.", result);
				return;
			}
		}

		if(!idx_buffer) {
			D3D11_BUFFER_DESC buffer_desc{
				.ByteWidth = (std::uint32_t)index_buffer.size() * sizeof(int),
				.Usage = D3D11_USAGE_DYNAMIC,
				.BindFlags = D3D11_BIND_INDEX_BUFFER,
				.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
				.MiscFlags = 0
			};

			if(auto result = render::directx11::shared.device->CreateBuffer(&buffer_desc, nullptr, &idx_buffer); FAILED(result)) {
				utils::logger(utils::e_log_type::error, "cant create index buffer, return code {}.", result);
				return;
			}
		}

		D3D11_MAPPED_SUBRESOURCE vtx_buffer_subresource{ }, idx_buffer_subresource{ };
		if(auto result = render::directx11::shared.context->Map(vtx_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_buffer_subresource); FAILED(result)) {
			utils::logger(utils::e_log_type::error, "map vertex buffer failed, return code {}.", result);
			return;
		}
		if(auto result = render::directx11::shared.context->Map(idx_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_buffer_subresource); FAILED(result)) {
			utils::logger(utils::e_log_type::error, "map index buffer failed, return code {}.", result);
			return;
		}

		std::ranges::move(vertex_buffer, (Rml::Vertex*)vtx_buffer_subresource.pData);
		std::ranges::move(index_buffer, (int*)idx_buffer_subresource.pData);

		render::directx11::shared.context->Unmap(vtx_buffer, 0);
		render::directx11::shared.context->Unmap(idx_buffer, 0);
	}

	void c_mesh::use() {
		std::uint32_t stride = sizeof(Rml::Vertex);
		std::uint32_t offset{ };
		render::directx11::shared.context->IASetInputLayout(input_layout);
		render::directx11::shared.context->IASetVertexBuffers(0, 1, &vtx_buffer, &stride, &offset);
		render::directx11::shared.context->IASetIndexBuffer(idx_buffer, DXGI_FORMAT_R32_UINT, 0);
		render::directx11::shared.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}