#pragma once
#include <RmlUi/Core/Vertex.h>
#include <backend/internal/mesh.h>

namespace null::rml::renderer::impl {
	class c_geometry_buffer {
	public:
		std::vector<std::span<Rml::Vertex>> vertex_buffers{ };
		std::vector<std::span<int>> index_buffers{ };
		size_t vertex_buffers_size{ }, index_buffers_size{ };

	public:
		void add_vertex_buffer(const std::span<Rml::Vertex>& vertex_buffer) { vertex_buffers.push_back(vertex_buffer); vertex_buffers_size += vertex_buffer.size(); }
		void add_index_buffer(const std::span<int>& index_buffer) { index_buffers.push_back(index_buffer); index_buffers_size += index_buffer.size(); }

	public:
		void clear() { vertex_buffers.clear(); index_buffers.clear(); vertex_buffers_size = 0; index_buffers_size = 0; }
	};

	class i_mesh : public render::backend::i_mesh {
	public:
		c_geometry_buffer geometry_buffer{ };

	public:
		void clear_geometry() override { geometry_buffer.clear(); }
	}; inline std::unique_ptr<i_mesh> mesh{ };
}