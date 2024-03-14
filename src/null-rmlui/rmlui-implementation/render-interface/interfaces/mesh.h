#pragma once
#include <RmlUi/Core/Vertex.h>

namespace null::rml::renderer {
	class i_mesh : public render::backend::i_mesh {
	public:
		std::span<const Rml::Vertex> vertex_buffer{ };
		std::span<const int> index_buffer{ };

	public:
		i_mesh(const std::span<const Rml::Vertex>& _vertex_buffer, const std::span<const int>& _index_buffer)
			: vertex_buffer(_vertex_buffer), index_buffer(_index_buffer) { }

	public:
		void clear_geometry() override { }
	};
}