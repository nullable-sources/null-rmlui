#version 330

uniform sampler2D sampler;
uniform vec4 color;
uniform vec2 uv_min;
uniform vec2 uv_max;

in vec2 frag_uv;

out vec4 out_color;

void main() {
    vec2 in_region = step(uv_min, frag_uv) * step(frag_uv, uv_max);
    out_color = texture(sampler, frag_uv).a * in_region.x * in_region.y * color;
}