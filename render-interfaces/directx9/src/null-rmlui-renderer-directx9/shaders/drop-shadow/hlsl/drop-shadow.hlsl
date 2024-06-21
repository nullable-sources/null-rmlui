struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;

float4 uv_limits : register(c0);
float4 shadow_color : register(c1);

float4 main(ps_input_t input) : COLOR {
    float2 in_region = step(uv_limits.xy, input.uv) * step(input.uv, uv_limits.zw);
    return tex2D(texture_sampler, input.uv).a * in_region.x * in_region.y * shadow_color;
}