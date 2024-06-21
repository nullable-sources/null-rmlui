#include "gradient.h"

namespace null::rml::directx9 {
    void c_gradient_shader::set_constants(const constants_t& constants) {
        std::array<color_t<float>, 16> colors{ };
        std::array<vec4_t<float>, 16> stops{ };

        for(auto [color, stop, pair] : std::views::zip(colors, stops, constants.stops)) {
            color = pair.first.cast<float>();
            stop.x = pair.second;
        }

        const float num_stops = constants.stops.size();
        const float function = (float)constants.function;
        pixel_shader->set_constant(0, &function);
        pixel_shader->set_constant(1, &num_stops);
        pixel_shader->set_constant(2, constants.p.coordinates.data());
        pixel_shader->set_constant(3, constants.v.coordinates.data());
        pixel_shader->set_constant(4, (float*)colors.data(), 16);
        pixel_shader->set_constant(20, (float*)stops.data(), 16);
    }
}