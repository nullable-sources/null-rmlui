#pragma once
#include <null-render-backend-opengl3.h>
#include <null-rmlui.h>

namespace null::rml::opengl3 {
	class c_mesh : public renderer::i_mesh {
	public: using i_mesh::i_mesh;
		render::opengl3::c_vertex_element<Rml::Vertex, 2, opengl::e_float, false, offsetof(Rml::Vertex, position)> position{ };
		render::opengl3::c_vertex_element<Rml::Vertex, 4, opengl::e_unsigned_byte, true, offsetof(Rml::Vertex, colour)> color{ };
		render::opengl3::c_vertex_element<Rml::Vertex, 2, opengl::e_float, false, offsetof(Rml::Vertex, tex_coord)> uv{ };
		std::uint32_t vao{ }, vbo{ }, ibo{ };

	public:
		void create() override;
		void destroy() override;

	public:
		void compile() override;

		void use() override;
	};
}