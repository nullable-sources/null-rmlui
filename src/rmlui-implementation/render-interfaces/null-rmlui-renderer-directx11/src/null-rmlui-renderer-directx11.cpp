#include <windows.h>
#include <null-render.h>

#include <null-rmlui-renderer-directx11.h>

namespace null::rml::renderer {
	void c_directx11::initialize() {
		i_render_interface::initialize();

		static const auto create_rasterizer_state{
			[](ID3D11RasterizerState** rasterizer_state, const bool& scissor_enable) {
				D3D11_RASTERIZER_DESC desc{
					.FillMode{ D3D11_FILL_SOLID },
					.CullMode{ D3D11_CULL_NONE },
					.DepthClipEnable{ scissor_enable },
					.ScissorEnable{ scissor_enable }
				};
				render::backend::directx11::shared.device->CreateRasterizerState(&desc, rasterizer_state);
			}
		};

		create_rasterizer_state(&disabled_scissor, false);
		create_rasterizer_state(&enabled_scissor, true);
	}

	void c_directx11::shutdown() {
		if(disabled_scissor) { disabled_scissor->Release(); disabled_scissor = nullptr; }
		if(enabled_scissor) { enabled_scissor->Release(); enabled_scissor = nullptr; }
	}
}