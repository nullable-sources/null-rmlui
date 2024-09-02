#pragma once
#include "../factory/factory.h"
#include "../../../interfaces/shaders/drop-shadow.h"

namespace ntl::rml::renderer::filters {
    class c_shadow_filter : public i_filter {
    public:
        float sigma{ };
        color_t<int> color{ };
        vec2_t<float> offset{ };

    public:
        c_shadow_filter() { }
        c_shadow_filter(float _sigma, const color_t<int>& _color, const vec2_t<float>& _offset)
            : sigma(_sigma), color(_color), offset(_offset) { }

    public:
        void render() override;
    };

    class c_shadow_filter_instancer : public i_filter_instancer {
    public:
        std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };
}