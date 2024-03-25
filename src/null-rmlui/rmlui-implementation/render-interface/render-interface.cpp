#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <null-render.h>

#include "../../compatibility/rect.h"
#include "../../compatibility/vec2.h"
#include "null-rmlui.h"
#include "interfaces/shaders/texture.h"
#include "interfaces/shaders/color.h"
#include "post-processing/layers.h"

namespace null::rml {
	void i_render_interface::SetScissorRegion(Rml::Rectanglei region) {
		scissor = region;
		render::backend::renderer->set_clip(rect_t<float>(region.p0.x, region.p0.y, region.p1.x, region.p1.y));
	}

	void i_render_interface::SetTransform(const Rml::Matrix4f* transform) {
		matrix4x4_t matrix = render::backend::renderer->get_projection_matrix();
		if(transform) matrix *= matrix4x4_t(*(std::array<float, 16>*)transform->data());
		render::backend::renderer->update_matrix(matrix);
	}

	void i_render_interface::RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture) {
		compiled_geometry_t* compiled_geometry = (compiled_geometry_t*)geometry;

		if(texture) {
			render::backend::state_pipeline->shaders.push(renderer::texture_shader);
			render::backend::state_pipeline->textures.push((void*)texture);
		} else {
			render::backend::state_pipeline->shaders.push(renderer::color_shader);
		}

		render::backend::state_pipeline->meshes.push(compiled_geometry->mesh);
		render::backend::renderer->update_translation(*(vec2_t<float>*)&translation);
		render::backend::renderer->draw_geometry(render::backend::e_topology::triangle_list, compiled_geometry->mesh->vertex_buffer.size(), compiled_geometry->mesh->index_buffer.size(), 0, 0);
		render::backend::state_pipeline->meshes.pop();

		if(texture) render::backend::state_pipeline->textures.pop();
		render::backend::state_pipeline->shaders.pop();
	}

	void i_render_interface::ReleaseGeometry(Rml::CompiledGeometryHandle geometry) {
		compiled_geometry_t* compiled_geometry = (compiled_geometry_t*)geometry;
		compiled_geometry->mesh->destroy();
		delete compiled_geometry;
	}

	Rml::TextureHandle i_render_interface::LoadTexture(Rml::Vector2i& texture_dimensions, const std::string& source) {
		Rml::FileInterface* file_interface = Rml::GetFileInterface();
		Rml::FileHandle file_handle = file_interface->Open(source);

		std::vector<uint8_t> file_data(file_interface->Length(file_handle));
		file_interface->Read(file_data.data(), file_data.size(), file_handle);
		file_interface->Close(file_handle);

		std::uint8_t* data = stbi_load_from_memory(file_data.data(), file_data.size(), &texture_dimensions.x, &texture_dimensions.y, nullptr, 4);
		std::unique_ptr<std::uint8_t[]> premultiplied_data = render::backend::renderer->premultiply_texture_color((vec2_t<int>)texture_dimensions, data);
		Rml::TextureHandle texture_handle = GenerateTexture({ premultiplied_data.get(), (size_t)(texture_dimensions.x * texture_dimensions.y * 4) }, texture_dimensions);
		stbi_image_free(data);

		return texture_handle;
	}

	Rml::TextureHandle i_render_interface::GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions) {
		return (Rml::TextureHandle)render::backend::renderer->create_texture(vec2_t<float>(source_dimensions.x, source_dimensions.y), (void*)source.data());
	}

	void i_render_interface::ReleaseTexture(Rml::TextureHandle texture) {
		render::backend::renderer->destroy_texture((void*)texture);
	}

	void i_render_interface::EnableClipMask(bool enable) {
		render::backend::stencil_buffer->set_test(enable);
	}

	void i_render_interface::RenderToClipMask(Rml::ClipMaskOperation operation, Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation) {
		if(operation == Rml::ClipMaskOperation::Set || operation == Rml::ClipMaskOperation::SetInverse) {
			render::backend::stencil_buffer->clear();
		}

		render::backend::stencil_buffer->set_operation((render::backend::e_stencil_operation)operation);
		RenderGeometry(geometry, translation, {});
		render::backend::stencil_buffer->set_operation(render::backend::e_stencil_operation::end_write);
	}

	void i_render_interface::PushLayer(Rml::LayerFill layer_fill) {
		render::backend::i_frame_buffer* source_layer = renderer::layers->top();

		if(layer_fill == Rml::LayerFill::Link) renderer::layers->push_clone();
		else renderer::layers->push();

		if(layer_fill == Rml::LayerFill::Copy)
			renderer::layers->top()->copy_from(source_layer);

		render::backend::state_pipeline->framebuffers.push(renderer::layers->top());
		if(layer_fill == Rml::LayerFill::Clear) renderer::layers->top()->clear();
	}

	void i_render_interface::PopLayer(Rml::BlendMode blend_mode, Rml::Span<const Rml::CompiledFilterHandle> filters) {
		render::backend::state_pipeline->framebuffers.pop();

		if(blend_mode == Rml::BlendMode::Discard) {
			renderer::layers->pop();
			render::backend::state_pipeline->framebuffers.pop();
			render::backend::state_pipeline->framebuffers.push(renderer::layers->top());
			return;
		}


		renderer::layers->blit_top();

		for(const Rml::CompiledFilterHandle filter_handle : filters) {
			const compiled_filter_t& filter = *(const compiled_filter_t*)(filter_handle);
			filter.filter->render();
		}

		renderer::layers->pop();

		if(blend_mode == Rml::BlendMode::Replace)
			set_blend_state(false);

		render::backend::state_pipeline->framebuffers.push(renderer::layers->top());
		render::backend::post_processing->blit_buffer(renderer::layers->primary());
		render::backend::state_pipeline->framebuffers.pop();

		if(blend_mode == Rml::BlendMode::Replace)
			set_blend_state(true);
	}

	Rml::TextureHandle i_render_interface::SaveLayerAsTexture(Rml::Vector2i dimensions) {
		Rml::TextureHandle render_texture = GenerateTexture({}, dimensions);
		if(!render_texture) return { };

		renderer::layers->blit_top();

		const rect_t<int> initial_scissor = scissor;
		EnableScissorRegion(false);

		rect_t<int> bounds = initial_scissor;
		if(render::backend::renderer->framebuffer_uvs_flipped()) {
			bounds.min.y = render::shared::viewport.y - initial_scissor.min.y;
			bounds.max.y = render::shared::viewport.y - initial_scissor.max.y;
		}

		renderer::layers->secondary()->blit_region_from(renderer::layers->primary(), { }, bounds);
		renderer::layers->secondary()->copy_in_texture((void*)render_texture, rect_t<int>(vec2_t<int>(0.f), initial_scissor.size()));

		SetScissorRegion(initial_scissor);

		return render_texture;
	}

	Rml::CompiledFilterHandle i_render_interface::SaveLayerAsMaskImage() {
		renderer::layers->blit_top();

		set_blend_state(false);
		render::backend::state_pipeline->framebuffers.push(renderer::layers->blend_mask());
		render::backend::post_processing->blit_buffer(renderer::layers->primary());
		render::backend::state_pipeline->framebuffers.pop();
		set_blend_state(true);

		renderer::i_filter_instancer* instancer = renderer::c_filter_factory::get_filter_instancer("mask-image");
		if(!instancer) {
			Rml::Log::Message(Rml::Log::LT_WARNING, "mask-image filter not registered.");
			return {};
		}

		return (Rml::CompiledFilterHandle)new compiled_filter_t(instancer->instance_filter({ }));
	}

	Rml::CompiledFilterHandle i_render_interface::CompileFilter(const Rml::String& name, const Rml::Dictionary& parameters) {
		renderer::i_filter_instancer* instancer = renderer::c_filter_factory::get_filter_instancer(name);
		if(!instancer) {
			Rml::Log::Message(Rml::Log::LT_WARNING, "Unsupported filter type '%s'.", name.c_str());
			return {};
		}

		return (Rml::CompiledFilterHandle)new compiled_filter_t(instancer->instance_filter(parameters));
	}

	void i_render_interface::ReleaseFilter(Rml::CompiledFilterHandle filter) {
		delete (compiled_filter_t*)(filter);
	}

	Rml::CompiledShaderHandle i_render_interface::CompileShader(const Rml::String& name, const Rml::Dictionary& parameters) {
		std::string fixed_name = name == "shader" ? Rml::Get(parameters, "value", Rml::String()) : name;
		renderer::i_geometry_filter_instancer* instancer = renderer::c_filter_factory::get_geometry_filter_instancer(fixed_name);
		if(!instancer) {
			Rml::Log::Message(Rml::Log::LT_WARNING, "'%s' geometry filter not registered.", fixed_name);
			return {};
		}

		return (Rml::CompiledFilterHandle)new compiled_geometry_filter_t(instancer->instance_filter(parameters));
	}

	void i_render_interface::RenderShader(Rml::CompiledShaderHandle shader_handle, Rml::CompiledGeometryHandle geometry_handle, Rml::Vector2f translation, Rml::TextureHandle texture) {
		const compiled_geometry_filter_t& filter = *(const compiled_geometry_filter_t*)(shader_handle);
		filter.filter->render(geometry_handle, translation, texture);
	}

	void i_render_interface::ReleaseShader(Rml::CompiledShaderHandle shader_handle) {
		delete reinterpret_cast<compiled_geometry_filter_t*>(shader_handle);
	}

	void i_render_interface::begin_render() {
		renderer::layers->push();
		render::backend::state_pipeline->framebuffers.push(renderer::layers->top());
		renderer::layers->top()->clear();
	}

	void i_render_interface::end_render() {
		render::backend::state_pipeline->framebuffers.pop();
		renderer::layers->blit_top();
		render::backend::post_processing->blit_buffer(renderer::layers->primary());

		renderer::layers->pop();

		scissor = rect_t<int>();
		render::backend::state_pipeline->setup_state();
	}
}
