#pragma once
#include <RmlUi/Core.h>
#include <null-render.h>

#include "interfaces/commands.h"

namespace null::rml {
	class i_render_interface : public Rml::RenderInterface {
	public:
		render::c_command_buffer command_buffer{ };

	public:
		void SetScissorRegion(int x, int y, int width, int height) override;
		void EnableScissorRegion(bool enable) override;
		void SetTransform(const Rml::Matrix4f* transform) override;

		void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;

		bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const std::string& source) override;
		bool GenerateTexture(Rml::TextureHandle& texture_handle, const byte* source, const Rml::Vector2i& source_dimensions) override;
		void ReleaseTexture(Rml::TextureHandle texture) override;

	public:
		virtual void initialize();
		virtual void shutdown() { }

		virtual void render();

	public:
		virtual std::unique_ptr<renderer::i_mesh> instance_mesh() = 0;
		virtual std::shared_ptr<renderer::c_geometry_command> instance_geometry_command(size_t index_count, size_t vertex_count, const Rml::Vector2f& translation, Rml::TextureHandle texture) { return std::make_shared<renderer::c_geometry_command>(index_count, vertex_count, translation, texture); }
		virtual std::shared_ptr<renderer::i_clip_enable_command> instatnce_clip_enable_command(bool enable) = 0;
	}; inline std::unique_ptr<i_render_interface> render_interface{ };
}