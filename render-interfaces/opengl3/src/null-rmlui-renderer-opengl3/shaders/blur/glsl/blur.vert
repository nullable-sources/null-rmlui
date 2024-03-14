#version 330
#define _blur_size 7
#define _num_weights ((_blur_size + 1) / 2)

uniform vec2 texel_offset;

in vec2 position;
in vec2 uv;
in vec4 color;

out vec2 frag_uv[_blur_size];

void main() {
	for(int i = 0; i < _blur_size; i++)
		frag_uv[i] = uv - float(i - _num_weights + 1) * texel_offset;
    gl_Position = vec4(position, 0.0, 1.0);
}