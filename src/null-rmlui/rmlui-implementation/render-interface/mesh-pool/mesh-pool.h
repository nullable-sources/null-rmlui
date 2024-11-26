#pragma once
#include <RmlUi/Core/Vertex.h>
#include <null-render/backend/internal/mesh.h>
#include "offsetAllocator.hpp"

namespace ntl::rml::renderer {
    class i_mesh_pool : public render::backend::i_mesh {
    public:
        static inline std::uint32_t pool_size{ 256 * 1024 };

    public:
        struct mesh_t {
        public:
            i_mesh_pool* pool{ };

            OffsetAllocator::Allocation vertex_allocation{ }, index_allocation{ };

            std::size_t vertex_count{ }, index_count{ };
            std::size_t vertex_offset{ }, index_offset{ };
        };

    protected:
        OffsetAllocator::Allocator vertex_allocator{ pool_size }, index_allocator{ pool_size };

    public:
        virtual bool create(mesh_t& mesh, Rml::Span<const Rml::Vertex>& vertex_buffer, Rml::Span<const int>& index_buffer) {
            mesh.vertex_allocation = vertex_allocator.allocate(vertex_buffer.size());
            mesh.index_allocation = index_allocator.allocate(index_buffer.size());
            mesh.vertex_offset = mesh.vertex_allocation.offset;
            mesh.vertex_count = vertex_buffer.size();
            mesh.index_offset = mesh.index_allocation.offset;
            mesh.index_count = index_buffer.size();
            mesh.pool = this;

            return mesh.vertex_allocation.offset != OffsetAllocator::Allocation::NO_SPACE && mesh.index_allocation.offset != OffsetAllocator::Allocation::NO_SPACE;
        }

        virtual void free(mesh_t& mesh) {
            vertex_allocator.free(mesh.vertex_allocation);
            index_allocator.free(mesh.index_allocation);
        }

    public:
        void clear_geometry() override { }
    };
}