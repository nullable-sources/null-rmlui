#pragma once
#include <null-render-backend-directx11.h>
#include <rmlui-implementation/render-interface/impl/mesh.h>

namespace null::rml::renderer::directx11::impl {
	class c_mesh : public renderer::impl::i_mesh {
	public:
		ID3D11InputLayout* input_layout{ };
		ID3D11Buffer* vertex_buffer{ }, *index_buffer{ };

	public:
		void create() override;
		void destroy() override;

		void compile() override;

		void set() override;
	};
}