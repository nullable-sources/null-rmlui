cbuffer constant_buffer : register(b0) {
    float4 uv_limits;
};

struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;
Texture2D texture_source;

float4 main(ps_input_t input) : SV_Target {
    return texture_source.Sample(texture_sampler, clamp(input.uv, uv_limits.xy, uv_limits.zw));
}