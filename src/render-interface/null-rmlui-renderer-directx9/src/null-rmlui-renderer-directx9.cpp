#include <windows.h>
#include <null-render.h>

#include <null-rmlui-renderer-directx9.h>

namespace null::rml::renderer {
	bool c_directx9::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) {		
		IDirect3DTexture9* texture{ };
		if(null::renderer::directx9->device->CreateTexture(source_dimensions.x, source_dimensions.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr) < 0)
			throw std::runtime_error{ "cannot create texture" };

		D3DLOCKED_RECT tex_locked_rect{ };
		if(int result{ texture->LockRect(0, &tex_locked_rect, nullptr, 0) }; result != D3D_OK)
			throw std::runtime_error{ std::format("lock rect error, code {}", result) };

		for(int size{ source_dimensions.x * 4 }; const int& y : std::views::iota(0, source_dimensions.y)) {
			std::memcpy((std::uint8_t*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, (std::uint8_t*)source + size * y, size);
		}

		texture->UnlockRect(0);
		texture_handle = (Rml::TextureHandle)texture;
		return texture;
	}

	void c_directx9::ReleaseTexture(Rml::TextureHandle texture_handle) {
		((IDirect3DTexture9*)texture_handle)->Release();
		texture_handle = Rml::TextureHandle{ };
	}

	void c_directx9::enable_scissor_region(const bool& enable) {
		null::renderer::directx9->device->SetRenderState(D3DRS_SCISSORTESTENABLE, enable);
	}

	void c_directx9::set_transform(const matrix4x4_t& transform) {
		null::renderer::directx9->device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)(matrix4x4_t::project_ortho(0.5f, null::renderer::draw_data_t::screen_size.x + 0.5f, null::renderer::draw_data_t::screen_size.y + 0.5f, 0.5f, -10000.f, 10000.f) * transform).linear_array.data());
	}

	Rml::TextureHandle c_directx9::convert_texture(const Rml::TextureHandle& texture) {
		return !texture ? Rml::TextureHandle{ } : texture;
	}
}