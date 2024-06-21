#version 330
#define _blur_size 7
#define _num_weights ((_blur_size + 1) / 2)

uniform sampler2D sampler;
uniform float weights[_num_weights];
uniform vec2 uv_min;
uniform vec2 uv_max;

in vec2 frag_uv[_blur_size];

out vec4 out_color;

void main() {
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    for(int i = 0; i < _blur_size; i++) {
        vec2 in_region = step(uv_min, frag_uv[i]) * step(frag_uv[i], uv_max);
        color += texture(sampler, frag_uv[i]) * in_region.x * in_region.y * weights[abs(i - _num_weights + 1)];
    }
    out_color = color;
}