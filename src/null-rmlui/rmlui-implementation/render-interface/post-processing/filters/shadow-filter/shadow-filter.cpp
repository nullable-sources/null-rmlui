#include "../../../../../compatibility/color.h"
#include "../../../../../compatibility/vec2.h"
#include "../../../../../compatibility/rect.h"

#include "../../../render-interface.h"
#include "../../layers.h"
#include "../blur-filter/blur-filter.h"
#include "shadow-filter.h"

namespace null::rml::renderer::filters {
    void c_shadow_filter::render() {
        render::backend::i_frame_buffer* primary = layers->primary();
        render::backend::i_frame_buffer* secondary = layers->secondary();

        render_interface->set_blend_state(false);

        i_drop_shadow_shader::constants_t shadow_constants{ };
        shadow_constants.from_region(render_interface->scissor);
        shadow_constants.color = color;
        drop_shadow_shader->set_constants(shadow_constants);
        render::backend::state_pipeline->shaders.push(drop_shadow_shader);
        render::backend::state_pipeline->framebuffers.push(secondary);

        const vec2_t<float> uv_offset = offset / vec2_t<float>(-(float)render::shared::viewport.x, (float)render::shared::viewport.y);
        render::backend::post_processing->generate_geometry(
            render::backend::c_post_processing::normalized_viewport,
            render::backend::c_post_processing::normalized_uvs + uv_offset
        );
        render::backend::post_processing->draw_buffer_texture(primary);

        render::backend::state_pipeline->shaders.pop();

        if(sigma >= 0.5f)
            c_blur_filter::make_blur(sigma, secondary, layers->tertiary());

        render_interface->set_blend_state(true);
        render::backend::post_processing->blit_buffer(primary);

        render::backend::state_pipeline->framebuffers.pop();

        layers->swap_primary_secondary();
    }

    std::unique_ptr<i_filter> c_shadow_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        return std::make_unique<c_shadow_filter>(
            Rml::Get(parameters, "sigma", 0.f),
            Rml::Get(parameters, "color", Rml::Colourb()).ToPremultiplied(),
            Rml::Get(parameters, "offset", Rml::Vector2f(0.f))
        );
    }
}