#pragma once
#include <RmlUi/Core.h>
#include <null-render.h>

#include "interfaces/mesh.h"
#include "post-processing/filters/factory/factory.h"

namespace null::rml {
	class i_render_interface : public Rml::RenderInterface {
	public:
		struct compiled_geometry_t {
		public:
			std::unique_ptr<renderer::i_mesh> mesh{ };

		public:
			compiled_geometry_t() { }
			compiled_geometry_t(std::unique_ptr<renderer::i_mesh>&& _mesh) : mesh(std::move(_mesh)) {
				mesh->create();
				mesh->compile();
			}
		};

		struct compiled_filter_t {
		public:
			std::unique_ptr<renderer::i_filter> filter{ };
		};

		struct compiled_geometry_filter_t {
		public:
			std::unique_ptr<renderer::i_geometry_filter> filter{ };
		};

	public:
		rect_t<int> scissor{ };

	public:
		virtual void set_secondary_texture(void* texture) = 0;
		virtual void set_blend_state(bool enabled) = 0;
		virtual void set_blend_function(bool enabled, float alpha) = 0;

	public:
		void SetScissorRegion(Rml::Rectanglei region) override;
		void SetTransform(const Rml::Matrix4f* transform) override;

		void RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture) override;
		void ReleaseGeometry(Rml::CompiledGeometryHandle geometry) override;

		Rml::TextureHandle LoadTexture(Rml::Vector2i& texture_dimensions, const std::string& source) override;
		Rml::TextureHandle GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions) override;
		void ReleaseTexture(Rml::TextureHandle texture) override;

		void EnableClipMask(bool enable) override;
		void RenderToClipMask(Rml::ClipMaskOperation mask_operation, Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation) override;

		Rml::LayerHandle PushLayer() override;
		void CompositeLayers(Rml::LayerHandle source, Rml::LayerHandle destination, Rml::BlendMode blend_mode, Rml::Span<const Rml::CompiledFilterHandle> filters) override;
		void PopLayer() override;

		Rml::TextureHandle SaveLayerAsTexture(Rml::Vector2i dimensions) override;
		Rml::CompiledFilterHandle SaveLayerAsMaskImage() override;

		Rml::CompiledFilterHandle CompileFilter(const Rml::String& name, const Rml::Dictionary& parameters) override;
		void ReleaseFilter(Rml::CompiledFilterHandle filter) override;

		Rml::CompiledShaderHandle CompileShader(const Rml::String& name, const Rml::Dictionary& parameters) override;
		void RenderShader(Rml::CompiledShaderHandle shader_handle, Rml::CompiledGeometryHandle geometry_handle, Rml::Vector2f translation, Rml::TextureHandle texture) override;
		void ReleaseShader(Rml::CompiledShaderHandle effect_handle) override;

	public:
		virtual void initialize() { }
		virtual void shutdown() { }

		virtual void begin_render();
		virtual void end_render();
	}; inline std::unique_ptr<i_render_interface> render_interface{ };
}