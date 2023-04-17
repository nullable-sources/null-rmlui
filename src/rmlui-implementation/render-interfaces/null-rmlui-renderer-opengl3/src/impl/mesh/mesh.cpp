#include <impl/shaders/compiled-objects/passthrough/passthrough.h>
#include <impl/mesh/mesh.h>

namespace null::rml::renderer::opengl3::impl {
    void c_mesh::create() {
        if(vao != 0) return;

        opengl::gen_vertex_arrays(1, &vao);
        opengl::gen_buffers(1, &vbo);
        opengl::gen_buffers(1, &ibo);
        opengl::bind_vertex_array(vao);

        render::backend::opengl3::wrapper::c_program program{ };
        program.create();

        program.attach_shader(&shaders::compiled_objects::passthrough);
        program.link();
        program.detach_shader(&shaders::compiled_objects::passthrough);

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
        opengl::buffer_data(opengl::e_array_buffer, geometry_buffer.vertex_buffers_size * sizeof(Rml::Vertex), nullptr, opengl::e_static_draw);

        size_t vertex_offset{ };
        for(const std::span<Rml::Vertex>& vertex_buffer : geometry_buffer.vertex_buffers) {
            opengl::buffer_sub_data(opengl::e_array_buffer, vertex_offset * sizeof(Rml::Vertex), vertex_buffer.size() * sizeof(Rml::Vertex), vertex_buffer.data());
            vertex_offset += vertex_buffer.size();
        }

        position.set();
        uv.set();
        color.set();

        opengl::bind_buffer(opengl::e_element_array_buffer, ibo);
        opengl::buffer_data(opengl::e_element_array_buffer, geometry_buffer.index_buffers_size * sizeof(int), nullptr, opengl::e_static_draw);
        
        size_t index_offset{ };
        for(const std::span<int>& index_buffer : geometry_buffer.index_buffers) {
            opengl::buffer_sub_data(opengl::e_element_array_buffer, index_offset * sizeof(int), index_buffer.size() * sizeof(int), index_buffer.data());
            index_offset += index_buffer.size();
        }

        opengl::bind_vertex_array(0);
    }

    void c_mesh::set() {
        opengl::bind_vertex_array(vao);
    }
}