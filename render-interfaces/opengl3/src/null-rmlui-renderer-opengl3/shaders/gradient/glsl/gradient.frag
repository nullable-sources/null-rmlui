#version 330
#define _linear 0
#define _radial 1
#define _conic 2
#define _repeating_linear 3
#define _repeating_radial 4
#define _repeating_conic 5
#define _pi 3.14159265

uniform int func; // one of the above definitions
uniform vec2 p;   // linear: starting point,         radial: center,                        conic: center
uniform vec2 v;   // linear: vector to ending point, radial: 2d curvature (inverse radius), conic: angled unit vector
uniform vec4 stop_colors[16];
uniform float stop_positions[16]; // normalized, 0 -> starting point, 1 -> ending point
uniform int num_stops;

in vec2 frag_uv;
in vec4 frag_color;

out vec4 out_color;

vec4 mix_stop_colors(float t) {
	vec4 color = stop_colors[0];

	for (int i = 1; i < num_stops; i++)
		color = mix(color, stop_colors[i], smoothstep(stop_positions[i-1], stop_positions[i], t));

	return color;
}

void main() {
	float t = 0.0;

	if(func == _linear || func == _repeating_linear) {
		float dist_square = dot(v, v);
		vec2 V = frag_uv - p;
		t = dot(v, V) / dist_square;
	} else if(func == _radial || func == _repeating_radial) {
		vec2 V = frag_uv - p;
		t = length(v * V);
	} else if(func == _conic || func == _repeating_conic) {
		mat2 R = mat2(v.x, -v.y, v.y, v.x);
		vec2 V = R * (frag_uv - p);
		t = 0.5 + atan(-V.x, V.y) / (2.0 * _pi);
	}

	if(func == _repeating_linear || func == _repeating_radial || func == _repeating_conic) {
		float t0 = stop_positions[0];
		float t1 = stop_positions[num_stops - 1];
		t = t0 + mod(t - t0, t1 - t0);
	}

	out_color = frag_color * mix_stop_colors(t);
}