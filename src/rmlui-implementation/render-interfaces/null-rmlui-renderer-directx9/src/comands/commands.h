#pragma once
#include <null-render-backend-directx9.h>
#include <null-rmlui.h>

namespace null::rml::directx9 {
	class c_clip_enable_command : public renderer::i_clip_enable_command {
	public: using i_clip_enable_command::i_clip_enable_command;
		void handle() override { render::directx9::shared.device->SetRenderState(D3DRS_SCISSORTESTENABLE, enable); }
	};
}