#pragma once
#include <RmlUi/Core.h>
#include <null-render.h>

#include "mesh-pool/mesh-pool.h"
#include "post-processing/filters/factory/factory.h"

namespace ntl::rml {
    class i_render_interface : public Rml::RenderInterface {
    public:
        struct compiled_filter_t {
        public:
            std::unique_ptr<renderer::i_filter> filter{ };
        };

        struct compiled_geometry_filter_t {
        public:
            std::unique_ptr<renderer::i_geometry_filter> filter{ };
        };

    public:
        std::vector<std::unique_ptr<renderer::i_mesh_pool>> mesh_pools{ };

        std::unique_ptr<render::backend::i_rasterizer_state> rasterizer_state_disabled_scissor{ };
        std::unique_ptr<render::backend::i_blend_state> blend_state_disabled{ }, blend_state_factor{ };
        rect_t<int> scissor{ };

    public:
        virtual std::unique_ptr<renderer::i_mesh_pool> instance_mesh_pool() = 0;
        virtual void set_secondary_texture(void* texture) = 0;

    public:
        void EnableScissorRegion(bool enable);
        void SetScissorRegion(Rml::Rectanglei region) override;
        void SetTransform(const Rml::Matrix4f* transform) override;

        Rml::CompiledGeometryHandle CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) override;
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

        Rml::TextureHandle SaveLayerAsTexture() override;
        Rml::CompiledFilterHandle SaveLayerAsMaskImage() override;

        Rml::CompiledFilterHandle CompileFilter(const Rml::String& name, const Rml::Dictionary& parameters) override;
        void ReleaseFilter(Rml::CompiledFilterHandle filter) override;

        Rml::CompiledShaderHandle CompileShader(const Rml::String& name, const Rml::Dictionary& parameters) override;
        void RenderShader(Rml::CompiledShaderHandle shader_handle, Rml::CompiledGeometryHandle geometry_handle, Rml::Vector2f translation, Rml::TextureHandle texture) override;
        void ReleaseShader(Rml::CompiledShaderHandle effect_handle) override;

    public:
        virtual void initialize();
        virtual void shutdown() { }

        virtual void begin_render();
        virtual void end_render();
    }; inline std::unique_ptr<i_render_interface> render_interface{ };
}