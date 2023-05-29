#pragma once
#include <RmlUi/Core.h>

#include <null-render.h>
#include <rmlui-implementation/render-interface/impl/commands.h>
#include <rmlui-implementation/render-interface/impl/shaders/passthrough.h>

namespace null::rml {
	class i_render_interface : public Rml::RenderInterface {
	public:
		render::c_draw_list draw_list{ };

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
		virtual std::unique_ptr<renderer::impl::i_mesh> instance_mesh() = 0;
		virtual std::unique_ptr<renderer::impl::commands::i_geometry> instance_geometry_command(const size_t& index_count, const size_t& vertex_count, const Rml::Vector2f& translation, const Rml::TextureHandle& texture) { { return std::make_unique<renderer::impl::commands::i_geometry>(index_count, vertex_count, translation, texture); } }
		virtual std::unique_ptr<renderer::impl::commands::i_clip_enable> instatnce_clip_enable_command(const bool& enable) = 0;

		virtual std::unique_ptr<renderer::impl::shaders::i_passthrough_color> instance_passthrough_color_shader() = 0;
		virtual std::unique_ptr<renderer::impl::shaders::i_passthrough_texture> instance_passthrough_texture_shader() = 0;
	}; inline std::unique_ptr<i_render_interface> render_interface{ };
}