#version 330
uniform vec2 translation;
uniform mat4 matrix;
uniform float depth;

in vec2 position;
in vec2 uv;
in vec4 color;

out vec2 frag_uv;
out vec4 frag_color;

void main() {
    frag_uv = uv;
    frag_color = color;
    gl_Position = matrix * vec4(position + translation, depth, 1.0);
}