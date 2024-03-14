#include "null-rmlui-renderer-directx11.h"

#include "null-rmlui-renderer-directx11/shaders/color/color.h"
#include "null-rmlui-renderer-directx11/shaders/texture/texture.h"
#include "null-rmlui-renderer-directx11/shaders/bliting/bliting.h"
#include "null-rmlui-renderer-directx11/shaders/color-matrix/color-matrix.h"
#include "null-rmlui-renderer-directx11/shaders/blend-mask/blend-mask.h"
#include "null-rmlui-renderer-directx11/shaders/gradient/gradient.h"
#include "null-rmlui-renderer-directx11/shaders/blur/blur.h"
#include "null-rmlui-renderer-directx11/shaders/drop-shadow/drop-shadow.h"

namespace null::rml::directx11 {
	void c_render::set_secondary_texture(void* texture) {
		render::directx11::shared.context->PSSetShaderResources(1, 1, (ID3D11ShaderResourceView**)&texture);
	}

	void c_render::set_blend_state(bool enabled) {
		render::directx11::shared.context->OMSetBlendState(enabled ? blend_enabled : blend_disabled, nullptr, -1);
	}

	void c_render::set_blend_function(bool enabled, float alpha) {
		if(!enabled) {
			std::array<float, 4> alpha_array = { 0.f, 0.f, 0.f, alpha };
			render::directx11::shared.context->OMSetBlendState(blend_factor, alpha_array.data(), -1);
		} else {
			render::directx11::shared.context->OMSetBlendState(blend_enabled, nullptr, -1);
		}
	}

	Rml::CompiledGeometryHandle c_render::CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) {
		return (Rml::CompiledGeometryHandle)new i_render_interface::compiled_geometry_t(std::make_unique<c_mesh>(vertices, indices));
	}

	void c_render::EnableScissorRegion(bool enable) {
		render::directx11::shared.context->RSSetState(enable ? enabled_scissor : disabled_scissor);
		if(!enable) scissor = rect_t<float>();
	}

	void c_render::initialize() {
		renderer::color_shader = std::make_unique<c_color_shader>();
		renderer::texture_shader = std::make_unique<c_texture_shader>();
		renderer::bliting_shader = std::make_unique<c_bliting_shader>();
		renderer::color_matrix_shader = std::make_unique<c_color_matrix_shader>();
		renderer::blend_mask_shader = std::make_unique<c_blend_mask_shader>();
		renderer::gradient_shader = std::make_unique<c_gradient_shader>();
		renderer::blur_shader = std::make_unique<c_blur_shader>();
		renderer::drop_shadow_shader = std::make_unique<c_drop_shadow_shader>();

		static const auto create_rasterizer_state{
			[](ID3D11RasterizerState** rasterizer_state, bool scissor_enable) {
				D3D11_RASTERIZER_DESC desc{
					.FillMode = D3D11_FILL_SOLID,
					.CullMode = D3D11_CULL_NONE,
					.DepthClipEnable = scissor_enable,
					.ScissorEnable = scissor_enable
				};
				render::directx11::shared.device->CreateRasterizerState(&desc, rasterizer_state);
			}
		};

		create_rasterizer_state(&disabled_scissor, false);
		create_rasterizer_state(&enabled_scissor, true);

		static const auto create_blend_state{
			[](ID3D11BlendState** blend_state, bool blend_enable, D3D11_BLEND src_blend, D3D11_BLEND dst_blend) {
				D3D11_BLEND_DESC desc{
					.AlphaToCoverageEnable{ false },
					.RenderTarget{
						{
							.BlendEnable{ blend_enable },
							.SrcBlend{ src_blend  },
							.DestBlend{ dst_blend },
							.BlendOp{ D3D11_BLEND_OP_ADD },
							.SrcBlendAlpha{ src_blend },
							.DestBlendAlpha{ dst_blend },
							.BlendOpAlpha{ D3D11_BLEND_OP_ADD },
							.RenderTargetWriteMask{ D3D11_COLOR_WRITE_ENABLE_ALL }
						}
					}
				};
				render::directx11::shared.device->CreateBlendState(&desc, blend_state);
			}
		};

		create_blend_state(&blend_disabled, false, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA);
		create_blend_state(&blend_enabled, true, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA);
		create_blend_state(&blend_factor, true, D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_ZERO);
	}

	void c_render::shutdown() {
		if(disabled_scissor) { disabled_scissor->Release(); disabled_scissor = nullptr; }
		if(enabled_scissor) { enabled_scissor->Release(); enabled_scissor = nullptr; }
	}
}