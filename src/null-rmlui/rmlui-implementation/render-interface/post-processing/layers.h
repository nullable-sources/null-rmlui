#pragma once
#include <null-render/backend/post-processing/post-processing.h>
#include <RmlUi/Core/Types.h>

namespace null::rml::renderer {
	class c_layers {
	public:
		int involved_layers{ };

		std::vector<std::shared_ptr<render::backend::i_frame_buffer>> layers{ };
		std::array<std::unique_ptr<render::backend::i_frame_buffer>, 4> post_process{ };

	public:
		void initialize();

	public:
		void push();
		void push_clone();
		void pop();

	public:
		render::backend::i_frame_buffer* top() { return layers[involved_layers - 1].get(); }
		render::backend::i_frame_buffer* primary() { return post_process[0].get(); }
		render::backend::i_frame_buffer* secondary() { return post_process[1].get(); }
		render::backend::i_frame_buffer* tertiary() { return post_process[2].get(); }
		render::backend::i_frame_buffer* blend_mask() { return post_process[3].get(); }

		void swap_primary_secondary() { std::swap(post_process[0], post_process[1]); }

		//@note: blit top layer to postprocess primary
		void blit_top() { primary()->copy_from(top()); }

	private:
		bool is_clone_of_below(int layer_index) const {
			return (layer_index >= 1 && layer_index < (int)layers.size() && layers[layer_index].get() == layers[layer_index - 1].get());
		}
	}; inline std::unique_ptr<c_layers> layers{ };
}