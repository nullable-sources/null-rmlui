#pragma once
#include <RmlUi/Core/Vertex.h>

namespace null::rml::renderer {
    class i_mesh : public render::backend::i_mesh {
    protected:
        std::span<const Rml::Vertex> vertex_buffer{ };
        std::span<const int> index_buffer{ };

    public:
        i_mesh() { }
        i_mesh(const std::span<const Rml::Vertex>& _vertex_buffer, const std::span<const int>& _index_buffer)
            : vertex_buffer(_vertex_buffer), index_buffer(_index_buffer) { }

    public:
        virtual void reassign(std::span<const Rml::Vertex>& _vertex_buffer, std::span<const int>& _index_buffer) {
            vertex_buffer = _vertex_buffer;
            index_buffer = _index_buffer;
        }

    public:
        void clear_geometry() override { }

    public:
        const size_t vertex_buffer_size() const { return vertex_buffer.size(); }
        const size_t index_buffer_size() const { return index_buffer.size(); }
    };
}