#include <null-render/backend/backend.h>
#include "layers.h"

namespace null::rml::renderer {
    void c_layers::initialize() {
        for(auto& buffer : post_process) {
            buffer = render::backend::factory->instance_frame_buffer(render::shared::viewport, render::backend::e_frame_buffer_type::postprocessing, render::backend::e_frame_buffer_flags::none);
        }
    }

    Rml::LayerHandle c_layers::push() {
        if(involved_layers == (int)layers.size()) {
            std::unique_ptr<render::backend::i_frame_buffer> framebuffer = render::backend::factory->instance_frame_buffer(render::shared::viewport, render::backend::e_frame_buffer_type::postprocessing, render::backend::e_frame_buffer_flags::msaa);
            framebuffer->create();
            layers.push_back(std::move(framebuffer));
        }

        involved_layers += 1;
        return top_layer_handle();
    }

    void c_layers::pop() {
        involved_layers -= 1;
    }
}