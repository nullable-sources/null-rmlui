#pragma once
#include <null-render-backend-directx9.h>
#include <rmlui-implementation/render-interface/impl/commands.h>

namespace null::rml::renderer::directx9::impl::commands {
	class c_clip_enable : public renderer::impl::commands::i_clip_enable {
	public:
		c_clip_enable(const bool& _enable) : i_clip_enable{ _enable } { }

	public:
		void handle() override { render::backend::directx9::shared.device->SetRenderState(D3DRS_SCISSORTESTENABLE, enable); }
	};
}