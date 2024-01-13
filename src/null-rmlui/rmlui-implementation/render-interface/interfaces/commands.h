#pragma once
#include "mesh.h"

namespace null::rml::renderer {
	class c_geometry_command : public render::i_geometry_command {
	public:
		Rml::Vector2f translation{ };
		Rml::TextureHandle texture{ };

	public:
		c_geometry_command(size_t _index_count, size_t _vertex_count, const Rml::Vector2f& _translation, Rml::TextureHandle _texture)
			: translation(_translation), texture(_texture), i_geometry_command(mesh->geometry_buffer.index_buffers_size, mesh->geometry_buffer.vertex_buffers_size, _index_count, _vertex_count) { }

	public:
		virtual void handle() override {
			if(texture) {
				render::backend::state_pipeline->shaders.push(render::backend::passthrough_texture_shader);
				render::backend::state_pipeline->textures.push((void*)texture);
			}

			render::backend::renderer->update_translation(*(vec2_t<float>*)&translation);
			render::backend::renderer->draw_geometry(render::backend::e_topology::triangle_list, vertex_count, index_count, vertex_offset, index_offset);

			if(texture) {
				render::backend::state_pipeline->textures.pop();
				render::backend::state_pipeline->shaders.pop();
			}
		}
	};

	class i_clip_enable_command : public render::i_command {
	public:
		bool enable{ };

	public:
		i_clip_enable_command(bool _enable) : enable(_enable) { }
	};
}