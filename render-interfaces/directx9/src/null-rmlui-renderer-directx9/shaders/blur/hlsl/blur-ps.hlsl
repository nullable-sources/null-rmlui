#define _blur_size 7
#define _num_weights ((_blur_size + 1) / 2)

struct ps_input_t {
    float4 position : POSITION0;
    float2 uv[_blur_size] : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;

//@note: Everything breaks if you use float4 (Most likely due to alignment).
//       I fuck shaders on dx9, so I don't intend to waste extra time just to reduce the number of variables
float2 uv_min : register(c0);
float2 uv_max : register(c1);
float4 weights[_num_weights] : register(c2);

float4 main(ps_input_t input) : COLOR {
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < _blur_size; i++) {
        float2 in_region = step(uv_min, input.uv[i]) * step(input.uv[i], uv_max);
        color += tex2D(texture_sampler, input.uv[i]) * in_region.x * in_region.y * weights[abs(i - _num_weights + 1)].x;
    }
    return color;
}