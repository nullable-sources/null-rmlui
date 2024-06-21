#include "gradient.h"

namespace null::rml::opengl3 {
    void c_gradient_shader::create() {
        if(!empty()) return;
        c_default_shader::create();

        function.get_location(program.get(), "func");
        p.get_location(program.get(), "p");
        v.get_location(program.get(), "v");
        colors.get_location(program.get(), "stop_colors");
        stops.get_location(program.get(), "stop_positions");
        num_stops.get_location(program.get(), "num_stops");
    }

    void c_gradient_shader::use() {
        if(empty()) return;
        c_default_shader::use();

        function.set();
        p.set();
        v.set();
        colors.set();
        stops.set();
        num_stops.set();
    }

    void c_gradient_shader::set_constants(const constants_t& constants) {
        num_stops.value() = constants.stops.size();
        for(auto [color, stop, pair] : std::views::zip(colors.value(), stops.value(), constants.stops)) {
            color = pair.first;
            stop = pair.second;
        }

        function.value() = (int)constants.function;
        p.value() = constants.p;
        v.value() = constants.v;
    }
}