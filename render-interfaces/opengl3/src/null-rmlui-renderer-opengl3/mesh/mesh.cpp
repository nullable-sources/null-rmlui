#include "../shaders/color/compiled-object.h"
#include "mesh.h"

namespace ntl::rml::opengl3 {
    void c_mesh::create() {
        if(vao != 0) return;

        opengl::gen_vertex_arrays(1, &vao);
        opengl::gen_buffers(1, &vbo);
        opengl::gen_buffers(1, &ibo);
        opengl::bind_vertex_array(vao);

        render::opengl3::c_program program{ };
        program.create();

        program.attach_shader(&color_vertex_shader_object);
        program.link();
        program.detach_shader(&color_vertex_shader_object);

        position.get_location(&program, "position");
        uv.get_location(&program, "uv");
        color.get_location(&program, "color");

        program.destroy();

        opengl::bind_vertex_array(0);
    }

    void c_mesh::destroy() {
        opengl::delete_vertex_arrays(1, &vao);
        opengl::delete_buffers(1, &vbo);
        opengl::delete_buffers(1, &ibo);
    }

    void c_mesh::compile() {
        opengl::bind_vertex_array(vao);

        opengl::bind_buffer(opengl::e_array_buffer, vbo);
        opengl::buffer_data(opengl::e_array_buffer, vertex_buffer.size() * sizeof(Rml::Vertex), vertex_buffer.data(), opengl::e_static_draw);

        position.set();
        uv.set();
        color.set();

        opengl::bind_buffer(opengl::e_element_array_buffer, ibo);
        opengl::buffer_data(opengl::e_element_array_buffer, index_buffer.size() * sizeof(int), index_buffer.data(), opengl::e_static_draw);

        opengl::bind_vertex_array(0);
    }

    void c_mesh::use() {
        opengl::bind_vertex_array(vao);
    }
}