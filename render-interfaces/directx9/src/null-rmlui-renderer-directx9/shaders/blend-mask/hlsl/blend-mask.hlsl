struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler : register(s0);
sampler mask_sampler : register(s1);

float4 main(ps_input_t input) : COLOR {
    float4 texture_color = tex2D(texture_sampler, input.uv);
    float mask_alpha = tex2D(mask_sampler, input.uv).a;
    return texture_color * mask_alpha;
}