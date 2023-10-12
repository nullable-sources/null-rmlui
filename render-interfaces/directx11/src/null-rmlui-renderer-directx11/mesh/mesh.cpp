#include <null-rmlui-renderer-directx11/shaders/shared-objects/passthrough/compiled-object.h>
#include "mesh.h"

namespace null::rml::directx11 {
	void c_mesh::on_create() {
		if(input_layout) return;

		D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,      0, (UINT)offsetof(Rml::Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,      0, (UINT)offsetof(Rml::Vertex, tex_coord),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, (UINT)offsetof(Rml::Vertex, colour), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		if(auto result{ render::directx11::shared.device->CreateInputLayout(desc, 3, sources::passthrough().data(), sources::passthrough().size(), &input_layout) }; FAILED(result))
			utils::logger(utils::e_log_type::error, "cant create vertex input layout, return code {}.", result);
	}

	void c_mesh::on_destroy() {
		if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
		if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
		if(input_layout) { input_layout->Release(); input_layout = nullptr; }
	}

	void c_mesh::compile() {
		static int vertex_buffer_size{ 5000 }, index_buffer_size{ 10000 };
		if(!vertex_buffer || vertex_buffer_size < geometry_buffer.vertex_buffers_size) {
			if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = nullptr; }
			vertex_buffer_size = geometry_buffer.vertex_buffers_size + 5000;
			D3D11_BUFFER_DESC buffer_desc{
				.ByteWidth{ vertex_buffer_size * sizeof(Rml::Vertex) },
				.Usage{ D3D11_USAGE_DYNAMIC },
				.BindFlags{ D3D11_BIND_VERTEX_BUFFER },
				.CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE },
				.MiscFlags{ 0 }
			};
			if(auto result{ render::directx11::shared.device->CreateBuffer(&buffer_desc, nullptr, &vertex_buffer) }; FAILED(result)) {
				utils::logger(utils::e_log_type::error, "cant create vertex buffer, return code {}.", result);
				return;
			}
		}

		if(!index_buffer || index_buffer_size < geometry_buffer.index_buffers_size) {
			if(index_buffer) { index_buffer->Release(); index_buffer = nullptr; }
			index_buffer_size = geometry_buffer.index_buffers_size + 10000;
			D3D11_BUFFER_DESC buffer_desc{
				.ByteWidth{ index_buffer_size * sizeof(int) },
				.Usage{ D3D11_USAGE_DYNAMIC },
				.BindFlags{ D3D11_BIND_INDEX_BUFFER },
				.CPUAccessFlags{ D3D11_CPU_ACCESS_WRITE },
				.MiscFlags{ 0 }
			};

			if(auto result{ render::directx11::shared.device->CreateBuffer(&buffer_desc, nullptr, &index_buffer) }; FAILED(result)) {
				utils::logger(utils::e_log_type::error, "cant create index buffer, return code {}.", result);
				return;
			}
		}

		D3D11_MAPPED_SUBRESOURCE vertex_buffer_subresource{ }, index_buffer_subresource{ };
		if(auto result{ render::directx11::shared.context->Map(vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vertex_buffer_subresource) }; FAILED(result)) {
			utils::logger(utils::e_log_type::error, "map vertex buffer failed, return code {}.", result);
			return;
		}
		if(auto result{ render::directx11::shared.context->Map(index_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &index_buffer_subresource) }; FAILED(result)) {
			utils::logger(utils::e_log_type::error, "map index buffer failed, return code {}.", result);
			return;
		}

		size_t vertex_offset{ };
		for(const std::span<Rml::Vertex>& vertex_buffer : geometry_buffer.vertex_buffers) {
			std::ranges::move(vertex_buffer, (Rml::Vertex*)vertex_buffer_subresource.pData + vertex_offset);
			vertex_offset += vertex_buffer.size();
		}

		size_t index_offset{ };
		for(const std::span<int>& index_buffer : geometry_buffer.index_buffers) {
			std::ranges::move(index_buffer, (int*)index_buffer_subresource.pData + index_offset);
			index_offset += index_buffer.size();
		}

		render::directx11::shared.context->Unmap(vertex_buffer, 0);
		render::directx11::shared.context->Unmap(index_buffer, 0);
	}

	void c_mesh::set() {
		std::uint32_t stride{ sizeof(Rml::Vertex) };
		std::uint32_t offset{ };
		render::directx11::shared.context->IASetInputLayout(input_layout);
		render::directx11::shared.context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
		render::directx11::shared.context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
		render::directx11::shared.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}