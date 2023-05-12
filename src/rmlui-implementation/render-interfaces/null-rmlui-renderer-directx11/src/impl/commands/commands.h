#pragma once
#include <null-render-backend-directx11.h>
#include <rmlui-implementation/render-interface/impl/commands.h>

namespace null::rml::renderer::directx11::impl::commands {
	class c_clip_enable : public renderer::impl::commands::i_clip_enable {
	public:
		ID3D11RasterizerState* state{ };

	public:
		c_clip_enable(const bool& _enable, ID3D11RasterizerState* _state) : i_clip_enable{ _enable }, state{ _state } { }

	public:
		void handle() override { render::backend::directx11::shared.context->RSSetState(state); }
	};
}