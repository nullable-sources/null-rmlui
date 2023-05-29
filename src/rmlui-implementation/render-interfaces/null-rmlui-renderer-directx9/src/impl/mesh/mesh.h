#pragma once
#include <renderer/renderer.h>
#include <rmlui-implementation/render-interface/impl/mesh.h>

namespace null::rml::renderer::directx9::impl {
	class c_mesh : public renderer::impl::i_mesh {
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