#pragma once
#include <RmlUi/Core.h>
#include <null-render.h>

#include "interfaces/mesh.h"
#include "post-processing/filters/factory/factory.h"

namespace ntl::rml {
    class i_render_interface : public Rml::RenderInterface {
    public:
        class c_mesh_pool {
        public:
            size_t last_used_index{ };
            std::list<std::unique_ptr<renderer::i_mesh>> meshes{ };

        public:
            ~c_mesh_pool() {
                for(auto it : std::views::iota(meshes.begin(), meshes.end()))
                    (*it)->destroy();
            }

        public:
            renderer::i_mesh* push(std::unique_ptr<renderer::i_mesh>&& new_object) {
                int index = last_used_index++;
                if(index >= meshes.size()) {
                    std::unique_ptr<renderer::i_mesh>& new_mesh = meshes.emplace_back(std::move(new_object));
                    new_mesh->create();
                    return new_mesh.get();
                }
                return std::next(meshes.begin(), index)->get();
            }

            void pop(renderer::i_mesh* mesh) {
                if(auto finded = std::ranges::find(meshes, mesh, std::bind(&std::unique_ptr<renderer::i_mesh>::get, std::placeholders::_1)); finded != meshes.end()) {
                    meshes.splice(meshes.end(), meshes, finded);
                    last_used_index--;
                }
            }
        } mesh_pool{ };

        struct compiled_geometry_t {
        public:
            renderer::i_mesh* mesh{ };

        public:
            compiled_geometry_t() { }
            compiled_geometry_t(renderer::i_mesh* _mesh) : mesh(_mesh) { }

        public:
            void compile_mesh(std::span<const Rml::Vertex> vertex_buffer, std::span<const int> index_buffer) {
                mesh->reassign(vertex_buffer, index_buffer);
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
        std::unique_ptr<render::backend::i_rasterizer_state> rasterizer_state_disabled_scissor{ };
        std::unique_ptr<render::backend::i_blend_state> blend_state_disabled{ }, blend_state_factor{ };
        rect_t<int> scissor{ };

    public:
        virtual void set_secondary_texture(void* texture) = 0;

    public:
        void EnableScissorRegion(bool enable);
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
        virtual void initialize();
        virtual void shutdown() { }

        virtual void begin_render();
        virtual void end_render();
    }; inline std::unique_ptr<i_render_interface> render_interface{ };
}