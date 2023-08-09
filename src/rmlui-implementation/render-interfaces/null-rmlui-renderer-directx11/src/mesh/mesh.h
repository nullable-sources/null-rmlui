#pragma once
#include <null-render-backend-directx11.h>
#include <null-rmlui.h>

namespace null::rml::directx11 {
	class c_mesh : public renderer::i_mesh {
	public:
		ID3D11InputLayout* input_layout{ };
		ID3D11Buffer* vertex_buffer{ }, *index_buffer{ };

	public:
		void on_create() override;
		void on_destroy() override;

	public:
		void compile() override;

		void set() override;
	};
}