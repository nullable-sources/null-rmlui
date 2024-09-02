#pragma once
#include "null-rmlui-renderer-opengl3/mesh/mesh.h"

namespace ntl::rml::opengl3 {
    class c_render : public i_render_interface {
    public:
        void set_secondary_texture(void* texture) override;

    public:
        Rml::CompiledGeometryHandle CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) override;

    public:
        void initialize() override;
    };
}