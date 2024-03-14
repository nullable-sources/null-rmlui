#pragma once
#include <null-render-backend-directx9.h>
#include <null-rmlui.h>

namespace null::rml::directx9 {
	class c_mesh : public renderer::i_mesh {
	public: using i_mesh::i_mesh;
		IDirect3DVertexDeclaration9* vertex_declaration{ };
		IDirect3DVertexBuffer9* vtx_buffer{ };
		IDirect3DIndexBuffer9* idx_buffer{ };

	public:
		void create() override;
		void destroy() override;

	public:
		void compile() override;

		void use() override;
	};
}