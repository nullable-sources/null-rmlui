#include "../../../render-interface.h"
#include "../../layers.h"
#include "passthrough-filter.h"

namespace null::rml::renderer::filters {
    void c_passthrough_filter::render() {
        render_interface->set_blend_function(false, blend_factor);

        render::backend::state_pipeline->framebuffers.push(layers->secondary());
        render::backend::post_processing->blit_buffer(layers->primary());
        render::backend::state_pipeline->framebuffers.pop();

        layers->swap_primary_secondary();

        render_interface->set_blend_function(true, 0.f);
    }

    std::unique_ptr<i_filter> c_opacity_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        return std::make_unique<c_passthrough_filter>(Rml::Get(parameters, "value", 1.0f));
    }
}