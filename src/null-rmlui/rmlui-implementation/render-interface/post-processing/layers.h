#pragma once
#include <null-render/backend/post-processing/post-processing.h>
#include <RmlUi/Core/Types.h>

namespace null::rml::renderer {
    class c_layers {
    public:
        int involved_layers{ };

        std::vector<std::unique_ptr<render::backend::i_frame_buffer>> layers{ };
        std::array<std::unique_ptr<render::backend::i_frame_buffer>, 4> post_process{ };

    public:
        void initialize();

    public:
        Rml::LayerHandle push();
        void pop();

    public:
        render::backend::i_frame_buffer* layer_by_handle(Rml::LayerHandle handle) { return layers[handle].get(); }
        Rml::LayerHandle top_layer_handle() const { return (Rml::LayerHandle)involved_layers - 1; }

        render::backend::i_frame_buffer* top() { return layer_by_handle(top_layer_handle()); }
        render::backend::i_frame_buffer* primary() { return post_process[0].get(); }
        render::backend::i_frame_buffer* secondary() { return post_process[1].get(); }
        render::backend::i_frame_buffer* tertiary() { return post_process[2].get(); }
        render::backend::i_frame_buffer* blend_mask() { return post_process[3].get(); }

        void swap_primary_secondary() { std::swap(post_process[0], post_process[1]); }

        //@note: blit top layer to postprocess primary
        void blit_top() { primary()->copy_from(top()); }
    }; inline std::unique_ptr<c_layers> layers{ };
}