#pragma once
#include <null-render-backend-opengl3.h>

#include <rmlui-implementation/render-interface/impl/mesh.h>

namespace null::rml::renderer::opengl3::impl {
	class c_mesh : public renderer::impl::i_mesh {
	public:
		render::backend::opengl3::wrapper::c_vertex_element<Rml::Vertex, 2, opengl::e_float, false, offsetof(Rml::Vertex, position)> position{ };
		render::backend::opengl3::wrapper::c_vertex_element<Rml::Vertex, 4, opengl::e_unsigned_byte, true, offsetof(Rml::Vertex, colour)> color{ };
		render::backend::opengl3::wrapper::c_vertex_element<Rml::Vertex, 2, opengl::e_float, false, offsetof(Rml::Vertex, tex_coord)> uv{ };
		std::uint32_t vao{ }, vbo{ }, ibo{ };

	public:
		void on_create() override;
		void on_destroy() override;

	public:
		void compile() override;

		void set() override;
	};
}