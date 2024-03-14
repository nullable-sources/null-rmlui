#define _blur_size 7
#define _num_weights ((_blur_size + 1) / 2)

cbuffer constant_buffer : register(b0) {
    float4 uv_limits;
    float4 weights[_num_weights];
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv[_blur_size] : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;
Texture2D texture_source;

float4 main(ps_input_t input) : SV_Target {
    float4 color = float4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < _blur_size; i++) {
        float2 in_region = step(uv_limits.xy, input.uv[i]) * step(input.uv[i], uv_limits.zw);
        color += texture_source.Sample(texture_sampler, input.uv[i]) * in_region.x * in_region.y * weights[abs(i - _num_weights + 1)].x;
    }
    return color;
}