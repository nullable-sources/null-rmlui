#version 330
uniform sampler2D sampler;
uniform mat4 color_matrix;

in vec2 frag_uv;
out vec4 out_color;

void main() {
	// The general case uses a 4x5 color matrix for full rgba transformation, plus a constant term with the last column.
	// However, we only consider the case of rgb transformations. Thus, we could in principle use a 3x4 matrix, but we
	// keep the alpha row for simplicity.
	// In the general case we should do the matrix transformation in non-premultiplied space. However, without alpha
	// transformations, we can do it directly in premultiplied space to avoid the extra division and multiplication
	// steps. In this space, the constant term needs to be multiplied by the alpha value, instead of unity.
	vec4 sampler_color = texture(sampler, frag_uv);
	vec3 transformed_color = vec3(color_matrix * sampler_color);
	out_color = vec4(transformed_color, sampler_color.a);
}