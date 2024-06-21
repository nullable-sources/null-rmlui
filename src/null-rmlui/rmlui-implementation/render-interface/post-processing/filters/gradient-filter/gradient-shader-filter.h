#pragma once
#include "../factory/factory.h"
#include "../../../interfaces/shaders/gradient.h"

namespace null::rml::renderer::filters {
    class c_gradient_shader_filter : public i_geometry_filter {
    public:
        static void stops_to_constants(i_gradient_shader::constants_t& constants, const Rml::ColorStopList& stops);

    public:
        i_gradient_shader::constants_t constants{ };

    public:
        c_gradient_shader_filter() { }
        c_gradient_shader_filter(const i_gradient_shader::constants_t& _constants) : constants(_constants) { }

    public:
        void render(Rml::CompiledGeometryHandle geometry_handle, const Rml::Vector2f& translation, Rml::TextureHandle texture) override;
    };

    class c_linear_gradient_shader_filter_instancer : public i_geometry_filter_instancer {
    public:
        std::unique_ptr<i_geometry_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };

    class c_radial_gradient_shader_filter_instancer : public i_geometry_filter_instancer {
    public:
        std::unique_ptr<i_geometry_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };

    class c_conic_gradient_shader_filter_instancer : public i_geometry_filter_instancer {
    public:
        std::unique_ptr<i_geometry_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };
}