#pragma once
#include <null-render-backend-directx11.h>
#include <null-rmlui.h>

namespace null::rml::directx11 {
	class c_clip_enable_command : public renderer::i_clip_enable_command {
	public:
		ID3D11RasterizerState* state{ };

	public:
		c_clip_enable_command(bool _enable, ID3D11RasterizerState* _state) : i_clip_enable_command(_enable), state(_state) { }

	public:
		void handle() override { render::directx11::shared.context->RSSetState(state); }
	};
}