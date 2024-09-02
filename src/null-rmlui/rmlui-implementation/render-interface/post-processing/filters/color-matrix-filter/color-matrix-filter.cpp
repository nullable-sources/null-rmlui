#include "../../../render-interface.h"
#include "../../layers.h"
#include "color-matrix-filter.h"

namespace ntl::rml::renderer::filters {
    void c_color_mask_filter::render() {
        render::backend::state_pipeline->blends.push(render_interface->blend_state_disabled);

        color_matrix_shader->set_constants(constants);
        render::backend::state_pipeline->shaders.push(color_matrix_shader);
        render::backend::state_pipeline->framebuffers.push(layers->secondary());
        render::backend::post_processing->draw_buffer(layers->primary());
        render::backend::state_pipeline->framebuffers.pop();
        render::backend::state_pipeline->shaders.pop();

        layers->swap_primary_secondary();

        render::backend::state_pipeline->blends.pop();
    }

    std::unique_ptr<i_filter> c_brightness_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        const float value = Rml::Get(parameters, "value", 1.0f);

        matrix4x4_t matrix{ };
        matrix.set_diagonal(vec4_t<float>(value, value, value, 1.f));
        return std::make_unique<c_color_mask_filter>(matrix);
    }

    std::unique_ptr<i_filter> c_contrast_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        const float value = Rml::Get(parameters, "value", 1.0f);
        const float grayness = 0.5f - 0.5f * value;

        matrix4x4_t matrix{ };
        matrix.set_diagonal(vec4_t<float>(value, value, value, 1.f));
        matrix.set_column(3, vec4_t<float>(grayness, grayness, grayness, 1.f));
        return std::make_unique<c_color_mask_filter>(matrix);
    }

    std::unique_ptr<i_filter> c_invert_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        const float value = Rml::Math::Clamp(Rml::Get(parameters, "value", 1.0f), 0.f, 1.f);
        const float inverted = 1.f - 2.f * value;

        matrix4x4_t matrix{ };
        matrix.set_diagonal(vec4_t<float>(inverted, inverted, inverted, 1.f));
        matrix.set_column(3, vec4_t<float>(value, value, value, 1.f));
        return std::make_unique<c_color_mask_filter>(matrix);
    }

    std::unique_ptr<i_filter> c_grayscale_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        const float value = Rml::Get(parameters, "value", 1.0f);
        const float rev_value = 1.f - value;
        const Rml::Vector3f gray = value * Rml::Vector3f(0.2126f, 0.7152f, 0.0722f);

        return std::make_unique<c_color_mask_filter>(matrix4x4_t(
            vec4_t<float>(gray.x + rev_value, gray.y, gray.z, 0.f),
            vec4_t<float>(gray.x, gray.y + rev_value, gray.z, 0.f),
            vec4_t<float>(gray.x, gray.y, gray.z + rev_value, 0.f),
            vec4_t<float>(0.f, 0.f, 0.f, 1.f)
        ));
    }

    std::unique_ptr<i_filter> c_sepia_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        const float value = Rml::Get(parameters, "value", 1.0f);
        const float rev_value = 1.f - value;
        const Rml::Vector3f r_mix = value * Rml::Vector3f(0.393f, 0.769f, 0.189f);
        const Rml::Vector3f g_mix = value * Rml::Vector3f(0.349f, 0.686f, 0.168f);
        const Rml::Vector3f b_mix = value * Rml::Vector3f(0.272f, 0.534f, 0.131f);

        return std::make_unique<c_color_mask_filter>(matrix4x4_t(
            vec4_t<float>(r_mix.x + rev_value, r_mix.y, r_mix.z, 0.f),
            vec4_t<float>(g_mix.x, g_mix.y + rev_value, g_mix.z, 0.f),
            vec4_t<float>(b_mix.x, b_mix.y, b_mix.z + rev_value, 0.f),
            vec4_t<float>(0.f, 0.f, 0.f, 1.f)
        ));
    }

    std::unique_ptr<i_filter> c_hue_rotate_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        const float value = Rml::Get(parameters, "value", 1.0f);
        const float s = Rml::Math::Sin(value);
        const float c = Rml::Math::Cos(value);

        return std::make_unique<c_color_mask_filter>(matrix4x4_t(
            vec4_t<float>(0.213f + 0.787f * c - 0.213f * s, 0.715f - 0.715f * c - 0.715f * s, 0.072f - 0.072f * c + 0.928f * s, 0.f),
            vec4_t<float>(0.213f - 0.213f * c + 0.143f * s, 0.715f + 0.285f * c + 0.140f * s, 0.072f - 0.072f * c - 0.283f * s, 0.f),
            vec4_t<float>(0.213f - 0.213f * c - 0.787f * s, 0.715f - 0.715f * c + 0.715f * s, 0.072f + 0.928f * c + 0.072f * s, 0.f),
            vec4_t<float>(0.f, 0.f, 0.f, 1.f)
        ));
    }

    std::unique_ptr<i_filter> c_saturate_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        const float value = Rml::Get(parameters, "value", 1.0f);

        return std::make_unique<c_color_mask_filter>(matrix4x4_t(
            vec4_t<float>(0.213f + 0.787f * value, 0.715f - 0.715f * value, 0.072f - 0.072f * value, 0.f),
            vec4_t<float>(0.213f - 0.213f * value, 0.715f + 0.285f * value, 0.072f - 0.072f * value, 0.f),
            vec4_t<float>(0.213f - 0.213f * value, 0.715f - 0.715f * value, 0.072f + 0.928f * value, 0.f),
            vec4_t<float>(0.f, 0.f, 0.f, 1.f)
        ));
    }
}