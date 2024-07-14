#include "../../../../../compatibility/color.h"
#include "../../../../../compatibility/vec2.h"
#include "../../../render-interface.h"
#include "gradient-shader-filter.h"

namespace null::rml::renderer::filters {
    void c_gradient_shader_filter::stops_to_constants(i_gradient_shader::constants_t& constants, const Rml::ColorStopList& stops) {
        for(const Rml::ColorStop& stop : stops)
            constants.stops.push_back({ color_t<int>(stop.color), stop.position.number });
    }

    void c_gradient_shader_filter::render(Rml::CompiledGeometryHandle geometry_handle, const Rml::Vector2f& translation, Rml::TextureHandle texture) {
        i_render_interface::compiled_geometry_t* geometry = (i_render_interface::compiled_geometry_t*)geometry_handle;

        gradient_shader->set_constants(constants);
        render::backend::state_pipeline->shaders.push(gradient_shader);
        render::backend::state_pipeline->meshes.push(geometry->mesh);

        render::backend::renderer->update_translation(*(vec2_t<float>*)&translation);
        render::backend::renderer->draw_geometry(render::backend::e_topology::triangle_list, geometry->mesh->vertex_buffer_size(), geometry->mesh->index_buffer_size(), 0, 0);

        render::backend::state_pipeline->meshes.pop();
        render::backend::state_pipeline->shaders.pop();
    }

    std::unique_ptr<i_geometry_filter> c_linear_gradient_shader_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        i_gradient_shader::constants_t constants{ };
        c_gradient_shader_filter::stops_to_constants(constants, Rml::GetIf(parameters, "color_stop_list")->GetReference<Rml::ColorStopList>());

        const bool repeating = Rml::Get(parameters, "repeating", false);
        constants.function = repeating ? i_gradient_shader::e_function::repeating_linear : i_gradient_shader::e_function::linear;
        constants.p = Rml::Get(parameters, "p0", Rml::Vector2f(0.f));
        constants.v = Rml::Get(parameters, "p1", Rml::Vector2f(0.f)) - constants.p;

        return std::make_unique<c_gradient_shader_filter>(constants);
    }

    std::unique_ptr<i_geometry_filter> c_radial_gradient_shader_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        i_gradient_shader::constants_t constants{ };
        c_gradient_shader_filter::stops_to_constants(constants, Rml::GetIf(parameters, "color_stop_list")->GetReference<Rml::ColorStopList>());

        const bool repeating = Rml::Get(parameters, "repeating", false);
        constants.function = repeating ? i_gradient_shader::e_function::repeating_radial : i_gradient_shader::e_function::radial;
        constants.p = Rml::Get(parameters, "center", Rml::Vector2f(0.f));
        constants.v = Rml::Vector2f(1.f) / Rml::Get(parameters, "radius", Rml::Vector2f(1.f));

        return std::make_unique<c_gradient_shader_filter>(constants);
    }

    std::unique_ptr<i_geometry_filter> c_conic_gradient_shader_filter_instancer::instance_filter(const Rml::Dictionary& parameters) {
        i_gradient_shader::constants_t constants{ };
        c_gradient_shader_filter::stops_to_constants(constants, Rml::GetIf(parameters, "color_stop_list")->GetReference<Rml::ColorStopList>());

        const bool repeating = Rml::Get(parameters, "repeating", false);
        constants.function = repeating ? i_gradient_shader::e_function::repeating_conic : i_gradient_shader::e_function::conic;
        constants.p = Rml::Get(parameters, "center", Rml::Vector2f(0.f));
        const float angle = Rml::Get(parameters, "angle", 0.f);
        constants.v = { Rml::Math::Cos(angle), Rml::Math::Sin(angle) };

        return std::make_unique<c_gradient_shader_filter>(constants);
    }
}