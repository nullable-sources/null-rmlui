#pragma once
#include <RmlUi/Core.h>

#include <null-render.h>

namespace null::rml {
	class i_render_interface : public Rml::RenderInterface {
	public:
		null::render::c_geometry_buffer buffer{ }; //@note: a separate layer for rendering the entire rmlui

	public:
		i_render_interface() { null::render::custom_buffers.push_back(&buffer); }

	public:
		void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation) override;
		
		void EnableScissorRegion(bool enable) override;
		void SetScissorRegion(int x, int y, int width, int height) override;

		bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const std::string& source) override;

		void SetTransform(const Rml::Matrix4f* transform) override;

	public:
		virtual void enable_scissor_region(const bool& enable) = 0;
		virtual void set_transform(const matrix4x4_t& transform) = 0;
	}; inline std::unique_ptr<i_render_interface> render_interface{ };
}