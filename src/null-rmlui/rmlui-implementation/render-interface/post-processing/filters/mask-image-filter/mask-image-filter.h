#pragma once
#include "../factory/factory.h"
#include "../../../interfaces/shaders/blend-mask.h"

namespace ntl::rml::renderer::filters {
    class c_mask_image_filter : public i_filter {
    public:
        void render() override;
    };

    class c_mask_image_filter_instancer : public i_filter_instancer {
    public:
        std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };
}