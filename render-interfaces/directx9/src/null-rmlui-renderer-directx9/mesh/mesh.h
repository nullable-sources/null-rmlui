#pragma once
#include <null-render-backend-directx9.h>
#include <null-rmlui.h>

namespace null::rml::directx9 {
	class c_mesh : public renderer::i_mesh {
	public:
		IDirect3DVertexDeclaration9* vertex_declaration{ };
		IDirect3DVertexBuffer9* vertex_buffer{ };
		IDirect3DIndexBuffer9* index_buffer{ };

	public:
		void on_create() override;
		void on_destroy() override;

	public:
		void compile() override;

		void set() override;
	};
}