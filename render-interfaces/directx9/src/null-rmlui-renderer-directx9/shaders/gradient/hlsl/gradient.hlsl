#define _linear 0
#define _radial 1
#define _conic 2
#define _repeating_linear 3
#define _repeating_radial 4
#define _repeating_conic 5
#define _pi 3.14159265

struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float func : register(c0); // one of the above definitions
float num_stops : register(c1);
float2 p : register(c2); // linear: starting point,         radial: center,                        conic: center
float2 v : register(c3); // linear: vector to ending point, radial: 2d curvature (inverse radius), conic: angled unit vector
float4 stop_colors[16] : register(c4);
float4 stop_positions[16] : register(c20); // normalized, 0 -> starting point, 1 -> ending point

float4 mix_stop_colors(float t) {
    float4 color = stop_colors[0];

    for (int i = 1; i < num_stops; i++)
        color = lerp(color, stop_colors[i], smoothstep(stop_positions[i - 1].x, stop_positions[i].x, t));

    return color;
}

float mod(float x, float y) { return x - y * floor(x / y); }

float4 main(ps_input_t input) : COLOR {
    float t = 0.0;

    if (func == _linear || func == _repeating_linear) {
        float dist_square = dot(v, v);
        float2 V = input.uv - p;
        t = dot(v, V) / dist_square;
    } else if (func == _radial || func == _repeating_radial) {
        float2 V = input.uv - p;
        t = length(v * V);
    } else if (func == _conic || func == _repeating_conic) {
        float2x2 R = float2x2(v.x, -v.y, v.y, v.x);
        float2 V = mul(R, input.uv - p);
        t = 0.5 + atan2(-V.x, V.y) / (2.0 * _pi);
    }

    if (func == _repeating_linear || func == _repeating_radial || func == _repeating_conic) {
        float t0 = stop_positions[0].x;
        float t1 = stop_positions[num_stops - 1].x;
        t = t0 + mod(t - t0, t1 - t0);
    }

    return input.color * mix_stop_colors(t);
}