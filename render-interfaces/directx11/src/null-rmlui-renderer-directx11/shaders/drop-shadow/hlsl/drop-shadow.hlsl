cbuffer constant_buffer : register(b0) {
    float4 uv_limits;
    float4 shadow_color;
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;
Texture2D texture_source;

float4 main(ps_input_t input) : SV_Target {    
    float2 in_region = step(uv_limits.xy, input.uv) * step(input.uv, uv_limits.zw);
    return texture_source.Sample(texture_sampler, input.uv).a * in_region.x * in_region.y * shadow_color;
}