#pragma once
#include <graphic/commands/geometry-command/geometry-command.h>
#include <rmlui-implementation/render-interface/impl/mesh.h>
#include <rmlui-implementation/render-interface/impl/shaders/passthrough.h>
#include <backend/shaders/passthrough/passthrough.h>
#include <backend/renderer/renderer.h>

namespace null::rml::renderer::impl::commands {
	class i_geometry : public render::commands::c_geometry {
	public:
		Rml::Vector2f translation{ };
		Rml::TextureHandle texture{ };

	public:
		i_geometry(const size_t& _index_count, const size_t& _vertex_count, const Rml::Vector2f& _translation, const Rml::TextureHandle& _texture)
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
				shaders::passthrough_texture->set_translation(translation);
				shaders::passthrough_texture->use();
			} else {
				shaders::passthrough_color->set_translation(translation);
				shaders::passthrough_color->use();
			}

			mesh->set();

			render::backend::renderer->draw_geometry(vertex_count, index_count, vertex_offset, index_offset);

			render::backend::mesh->set();
			render::backend::shaders::passthrough_color->use();
		}
	};

	class i_clip_enable : public render::commands::i_command {
	public:
		bool enable{ };

	public:
		i_clip_enable(const bool& _enable) : enable{ _enable } { }
	};
}