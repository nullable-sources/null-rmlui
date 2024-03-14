#pragma once
#include <null-render-backend-directx11.h>
#include <null-rmlui.h>

namespace null::rml::directx11 {
	class c_mesh : public renderer::i_mesh {
	public: using i_mesh::i_mesh;
		ID3D11InputLayout* input_layout{ };
		ID3D11Buffer* vtx_buffer{ }, *idx_buffer{ };

	public:
		void create() override;
		void destroy() override;

	public:
		void compile() override;

		void use() override;
	};
}