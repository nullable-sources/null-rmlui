#include "../../../render-interface.h"
#include "../../layers.h"
#include "passthrough-filter.h"

namespace ntl::rml::renderer::filters {
    void c_passthrough_filter::render() {
        render_interface->blend_state_factor->unlock();
        render_interface->blend_state_factor->factor.set(blend_factor);
        render_interface->blend_state_factor->lock();
        render::backend::state_pipeline->blends.push(render_interface->blend_state_factor);

        render::backend::state_pipeline->framebuffers.push(layers->secondary());
        render::backend::post_processing->blit_buffer(layers->primary());
        render::backend::state_pipeline->framebuffers.pop();

        layers->swap_primary_secondary();

        render::backend::state_pipeline->blends.pop();
    }

    std::unique_ptr<i_filter> c_opacity_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        return std::make_unique<c_passthrough_filter>(Rml::Get(parameters, "value", 1.0f));
    }
}