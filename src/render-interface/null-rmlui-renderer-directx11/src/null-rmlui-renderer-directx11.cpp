#include <windows.h>
#include <null-render.h>
#include <shaders/shaders.h>

#include <null-rmlui-renderer-directx11.h>

namespace null::rml::renderer {
    bool c_directx11::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) {
		D3D11_TEXTURE2D_DESC texture_desc{
			.Width{ (std::uint32_t)source_dimensions.x },
			.Height{ (std::uint32_t)source_dimensions.y },
			.MipLevels{ 1 },
			.ArraySize{ 1 },
			.Format{ DXGI_FORMAT_R8G8B8A8_UNORM },
			.SampleDesc{
				.Count{ 1 }
			},
			.Usage{ D3D11_USAGE_DEFAULT },
			.BindFlags{ D3D11_BIND_SHADER_RESOURCE },
		};

		D3D11_SUBRESOURCE_DATA subresource_data{
			.pSysMem{ source },
			.SysMemPitch{ source_dimensions.x * 4u }
		};

		ID3D11Texture2D* texture{ };
		if(null::renderer::directx11->device->CreateTexture2D(&texture_desc, &subresource_data, &texture) != S_OK)
			throw std::runtime_error{ "CreateTexture2D cant create texture" };

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc{
			.Format{ texture_desc.Format },
			.ViewDimension{ D3D11_SRV_DIMENSION_TEXTURE2D },
			.Texture2D{
				.MipLevels{ 1 }
			}
		};

		if(null::renderer::directx11->device->CreateShaderResourceView(texture, &srv_desc, (ID3D11ShaderResourceView**)&texture_handle) != S_OK)
			throw std::runtime_error{ "CreateShaderResourceView cant create shader resource view" };

		texture->Release();

        return (ID3D11ShaderResourceView*)texture_handle != nullptr;
    }

    void c_directx11::ReleaseTexture(Rml::TextureHandle texture_handle) {
		((ID3D11ShaderResourceView*)texture_handle)->Release();
		texture_handle = Rml::TextureHandle{ };
    }

    void c_directx11::enable_scissor_region(const bool& enable) {
		static const auto create_rasterizer_state{
			[](ID3D11RasterizerState** rasterizer_state, const bool& scissor_enable) {
				D3D11_RASTERIZER_DESC desc{
					.FillMode{ D3D11_FILL_SOLID },
					.CullMode{ D3D11_CULL_NONE },
					.DepthClipEnable{ scissor_enable },
					.ScissorEnable{ scissor_enable }
				};
				null::renderer::directx11->device->CreateRasterizerState(&desc, rasterizer_state);
			}
		};

		if(!disabled_scissor) create_rasterizer_state(&disabled_scissor, false);
		if(!enabled_scissor) create_rasterizer_state(&enabled_scissor, true);
        null::renderer::directx11->context->RSSetState(enable ? enabled_scissor : disabled_scissor);
    }

    void c_directx11::set_transform(const matrix4x4_t& transform) {
        null::render::shaders::vertex::shader.edit_constant(null::render::shaders::vertex::constant{ matrix4x4_t::project_ortho(0.f, null::renderer::draw_data_t::screen_size.x, null::renderer::draw_data_t::screen_size.y, 0.f, -10000.f, 10000.f) * transform });
    }

    Rml::TextureHandle c_directx11::convert_texture(const Rml::TextureHandle& texture) {
		if(!texture) {
			if(!empty_texture) GenerateTexture(empty_texture, std::vector<std::uint8_t>(4, 0xFF).data(), { 1, 1 });
			return empty_texture;
		}
        return texture;
    }
}