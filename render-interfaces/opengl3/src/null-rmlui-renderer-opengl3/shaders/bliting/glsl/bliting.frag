#version 330
uniform sampler2D sampler;
uniform vec2 uv_min;
uniform vec2 uv_max;

in vec2 frag_uv;

out vec4 out_color;

void main() {
	out_color = texture(sampler, clamp(frag_uv, uv_min, uv_max));
}