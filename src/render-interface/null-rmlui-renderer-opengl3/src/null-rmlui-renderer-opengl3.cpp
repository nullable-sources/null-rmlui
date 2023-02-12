#include <windows.h>
#include <null-render.h>

#include <null-rmlui-renderer-opengl3.h>

namespace null::rml::renderer {
	bool c_opengl3::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) {
		int last_texture{ }; opengl::get_integerv(opengl::e_texture_binding_2d, &last_texture);
		opengl::gen_textures(1, (std::uint32_t*)&texture_handle);
		opengl::bind_texture(opengl::e_texture_2d, texture_handle);
		opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_min_filter, opengl::e_linear);
		opengl::tex_parameteri(opengl::e_texture_2d, opengl::e_texture_mag_filter, opengl::e_linear);
		opengl::tex_image2d(opengl::e_texture_2d, 0, opengl::e_rgba, source_dimensions.x, source_dimensions.y, 0, opengl::e_rgba, opengl::e_unsigned_byte, (void*)source);

		opengl::bind_texture(opengl::e_texture_2d, last_texture);

		return texture_handle;
	}

	void c_opengl3::ReleaseTexture(Rml::TextureHandle texture_handle) {
		opengl::delete_textures(0, (std::uint32_t*)&texture_handle);
	}

	void c_opengl3::enable_scissor_region(const bool& enable) {
		enable ? opengl::enable(opengl::e_scissor_test) : opengl::disable(opengl::e_scissor_test);
	}

	void c_opengl3::set_transform(const matrix4x4_t& transform) {
		opengl::uniform_matrix4fv(null::renderer::opengl3->attribute_proj_mtx, 1, false, (matrix4x4_t::project_ortho(0.f, null::renderer::draw_data_t::screen_size.x, null::renderer::draw_data_t::screen_size.y, 0.f, -10000.f, 10000.f) * transform).linear_array.data());
	}

	Rml::TextureHandle c_opengl3::convert_texture(const Rml::TextureHandle& texture) {
		if(!texture) {
			if(!empty_texture) GenerateTexture(empty_texture, std::vector<std::uint8_t>(4, 0xFF).data(), { 1, 1 });
			return empty_texture;
		}
		return texture;
	}
}