#pragma once
#include "../factory/factory.h"
#include "../../../interfaces/shaders/color-matrix.h"

namespace null::rml::renderer::filters {
    class c_color_mask_filter : public i_filter {
    public:
        i_color_matrix_shader::constants_t constants{ };

    public:
        c_color_mask_filter() { }
        c_color_mask_filter(const matrix4x4_t& matrix) : constants(matrix) { }

    public:
        void render() override;
    };

    class c_brightness_filter_instancer : public i_filter_instancer {
    public:
        std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };

    class c_contrast_filter_instancer : public i_filter_instancer {
    public:
        std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };

    class c_invert_filter_instancer : public i_filter_instancer {
    public:
        std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };

    class c_grayscale_filter_instancer : public i_filter_instancer {
    public:
        std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };

    class c_sepia_filter_instancer : public i_filter_instancer {
    public:
        std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };

    class c_hue_rotate_filter_instancer : public i_filter_instancer {
    public:
        std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };

    class c_saturate_filter_instancer : public i_filter_instancer {
    public:
        std::unique_ptr<i_filter> instance_filter(const Rml::Dictionary& parameters) override;
    };
}