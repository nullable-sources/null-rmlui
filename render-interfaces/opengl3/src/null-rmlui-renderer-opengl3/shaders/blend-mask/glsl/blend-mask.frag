#version 420
layout(binding = 0) uniform sampler2D texture_sampler;
layout(binding = 1) uniform sampler2D mask_sampler;

in vec2 frag_uv;
out vec4 out_color;

void main() {
	vec4 texture_color = texture(texture_sampler, frag_uv);
	float mask_alpha = texture(mask_sampler, frag_uv).a;
	out_color = texture_color * mask_alpha;
}