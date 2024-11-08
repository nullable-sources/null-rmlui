#include "../../../../../compatibility/rect.h"
#include "../../../../../compatibility/vec2.h"
#include "../../../render-interface.h"
#include "../../layers.h"
#include "blur-filter.h"

namespace ntl::rml::renderer::filters {
    void c_blur_filter::calculate_weights(std::vector<float>& weights, float sigma) {
        const double double_pi_sqrt = std::sqrt(2.f * std::numbers::pi) * sigma;
        const double double_sigma = 2.0f * std::pow(sigma, 2);

        float normalization = 0.0f;
        for(int i : std::views::iota(0, i_blur_shader::_num_weights)) {
            float weight{ };
            if(std::abs(sigma) < 0.1f) weight = float(i == 0);
            else weight = std::exp(-float(i * i) / double_sigma) / double_pi_sqrt;

            normalization += (i == 0 ? 1.f : 2.0f) * weight;
            weights.push_back(weight);
        }

        std::ranges::transform(weights, weights.begin(), std::bind(std::divides<float>(), std::placeholders::_1, normalization));
    }

    void c_blur_filter::make_blur(float sigma, render::backend::i_frame_buffer* src, render::backend::i_frame_buffer* temp) {
        constexpr float max_single_pass_sigma = 3.0f;
        constexpr int max_num_passes = 10;
        static_assert(max_num_passes < 31, "");
        int pass_level = Rml::Math::Clamp(Rml::Math::Log2(int(sigma * (2.f / max_single_pass_sigma))), 0, max_num_passes);
        sigma = Rml::Math::Clamp(sigma / float(1 << pass_level), 0.0f, max_single_pass_sigma);

        const rect_t<int> original_scissor = render_interface->scissor;
        rect_t<int> scissor = original_scissor;

        const vec2_t<float> uv_scaling(
            (render::shared::viewport.x % 2 == 1) ? (1.f - 1.f / float(render::shared::viewport.x)) : 1.f,
            (render::shared::viewport.y % 2 == 1) ? (1.f - 1.f / float(render::shared::viewport.y)) : 1.f
        );

        //@note: downscaling texture
        for(int i : std::views::iota(0, pass_level)) {
            const bool from_source = (i % 2 == 0);
            scissor.min = (scissor.min + 1) / 2;
            scissor.max = math::max(scissor.max / 2, scissor.min);

            render::backend::renderer->set_clip(scissor);
            render::backend::state_pipeline->framebuffers.push(from_source ? temp : src);
            render::backend::post_processing->blit_buffer_region(from_source ? src : temp, uv_scaling * 2.f);
            render::backend::state_pipeline->framebuffers.pop();
        }

        if(pass_level % 2 == 0) {
            render::backend::state_pipeline->framebuffers.push(temp);
            render::backend::post_processing->blit_buffer(src);
            render::backend::state_pipeline->framebuffers.pop();
        }

        i_blur_shader::constants_t blur_constants{ };
        blur_constants.from_region(scissor);
        calculate_weights(blur_constants.weights, sigma);

        //@note: vertical pass
        blur_constants.texel_offset = vec2_t<float>(0.f, 1.f) * (1.f / float(render::shared::viewport.y));
        blur_shader->set_constants(blur_constants);
        render::backend::state_pipeline->shaders.push(blur_shader);
        render::backend::state_pipeline->framebuffers.push(src);
        render::backend::post_processing->draw_buffer(temp);
        render::backend::state_pipeline->framebuffers.pop();
        render::backend::state_pipeline->shaders.pop();

        //@note: horizontal pass
        blur_constants.texel_offset = vec2_t<float>(1.f, 0.f) * (1.f / float(render::shared::viewport.x));
        blur_shader->set_constants(blur_constants);
        render::backend::state_pipeline->shaders.push(blur_shader);
        render::backend::state_pipeline->framebuffers.push(temp);
        render::backend::post_processing->draw_buffer(src);
        render::backend::state_pipeline->framebuffers.pop();
        render::backend::state_pipeline->shaders.pop();

        //@note: bliting in src framebuffer.
        //       The example uses glBlitFramebuffer, but this function is only available in gl/dx9,
        //       in dx11 there is only CopySubresourceRegion, so we have to mess with shaders.
        //       uv_limits for blit is needed to avoid unnecessary darkening at the edges of the texture
        bliting_shader->set_constants(blur_constants);
        render::backend::state_pipeline->framebuffers.push(src);
        render::backend::state_pipeline->shaders.push(bliting_shader);

        render::backend::renderer->set_clip(original_scissor);
        render::backend::post_processing->draw_buffer_region(temp, vec2_t<float>(scissor.max) / vec2_t<float>(original_scissor.max));
        const rect_t<int> target = scissor * (1 << pass_level);
        if(target != original_scissor) {
            render::backend::renderer->set_clip(target);
            render::backend::post_processing->draw_buffer_region(temp, vec2_t<float>(scissor.max) / vec2_t<float>(target.max));
        }

        render::backend::state_pipeline->shaders.pop();
        render::backend::state_pipeline->framebuffers.pop();

        render::backend::renderer->set_clip(original_scissor);
    }

    void c_blur_filter::render() {
        render::backend::state_pipeline->blends.push(render_interface->blend_state_disabled);
        make_blur(sigma, layers->primary(), layers->secondary());
        render::backend::state_pipeline->blends.pop();
    }

    std::unique_ptr<i_filter> c_blur_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        return std::make_unique<c_blur_filter>(Rml::Get(parameters, "sigma", 1.0f));
    }
}