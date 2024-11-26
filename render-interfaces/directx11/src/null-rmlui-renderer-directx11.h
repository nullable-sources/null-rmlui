#pragma once
#include "null-rmlui-renderer-directx11/mesh-pool/mesh-pool.h"

namespace ntl::rml::directx11 {
    class c_render : public i_render_interface {
    public:
        void set_secondary_texture(void* texture) override;
        std::unique_ptr<renderer::i_mesh_pool> instance_mesh_pool() override;

    public:
        void initialize();
    };
}