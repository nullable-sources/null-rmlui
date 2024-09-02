#include "../../../render-interface.h"
#include "../../layers.h"
#include "mask-image-filter.h"

namespace ntl::rml::renderer::filters {
    void c_mask_image_filter::render() {
        render::backend::state_pipeline->blends.push(render_interface->blend_state_disabled);

        render_interface->set_secondary_texture(layers->blend_mask()->get_texture());
        render::backend::state_pipeline->shaders.push(blend_mask_shader);
        render::backend::state_pipeline->framebuffers.push(layers->secondary());
        render::backend::post_processing->draw_buffer(layers->primary());
        render::backend::state_pipeline->framebuffers.pop();
        render::backend::state_pipeline->shaders.pop();

        layers->swap_primary_secondary();

        render::backend::state_pipeline->blends.pop();
    }

    std::unique_ptr<i_filter> c_mask_image_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        return std::make_unique<c_mask_image_filter>();
    }
}