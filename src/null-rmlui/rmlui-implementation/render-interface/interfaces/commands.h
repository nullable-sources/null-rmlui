#pragma once
#include "mesh.h"
#include "shaders.h"

namespace null::rml::renderer {
	//@note: use before starting drawing via i_render_interface::draw_list
	class c_restore_command : public render::i_command {
	public:
		void handle() override {
			render::backend::mesh->set();
			render::backend::passthrough_color_shader->use();
		}
	};

	class i_geometry_command : public render::c_geometry_command {
	public:
		Rml::Vector2f translation{ };
		Rml::TextureHandle texture{ };

	public:
		i_geometry_command(size_t _index_count, size_t _vertex_count, const Rml::Vector2f& _translation, Rml::TextureHandle _texture)
			: translation{ _translation }, texture{ _texture } {
			index_count = _index_count;
			vertex_count = _vertex_count;
			index_offset = mesh->geometry_buffer.index_buffers_size;
			vertex_offset = mesh->geometry_buffer.vertex_buffers_size;
		}

	public:
		virtual void handle() override {
			if(texture) {
				render::backend::renderer->set_texture((void*)texture);
				passthrough_texture_shader->set_translation(translation);
				passthrough_texture_shader->use();
			} else {
				passthrough_color_shader->set_translation(translation);
				passthrough_color_shader->use();
			}
			mesh->set();

			render::backend::renderer->draw_geometry(vertex_count, index_count, vertex_offset, index_offset);
		}
	};

	class i_clip_enable_command : public render::i_command {
	public:
		bool enable{ };

	public:
		i_clip_enable_command(bool _enable) : enable{ _enable } { }
	};
}