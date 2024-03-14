struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;
Texture2D texture_source : register(t0);
Texture2D mask_source : register(t1);

float4 main(ps_input_t input) : SV_Target {
    float4 texture_color = texture_source.Sample(texture_sampler, input.uv);
    float mask_alpha = mask_source.Sample(texture_sampler, input.uv).a;
    return texture_color * mask_alpha;
}