#include "../../../render-interface.h"
#include "../../layers.h"
#include "mask-image-filter.h"

namespace null::rml::renderer::filters {
    void c_mask_image_filter::render() {
        render_interface->set_blend_state(false);

        render_interface->set_secondary_texture(layers->blend_mask()->get_texture());
        render::backend::state_pipeline->shaders.push(blend_mask_shader);
        render::backend::state_pipeline->framebuffers.push(layers->secondary());
        render::backend::post_processing->draw_buffer(layers->primary());
        render::backend::state_pipeline->framebuffers.pop();
        render::backend::state_pipeline->shaders.pop();

        layers->swap_primary_secondary();

        render_interface->set_blend_state(true);
    }

    std::unique_ptr<i_filter> c_mask_image_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        return std::make_unique<c_mask_image_filter>();
    }
}