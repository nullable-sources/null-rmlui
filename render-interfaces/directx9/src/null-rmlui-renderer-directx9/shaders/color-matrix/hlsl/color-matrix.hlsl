struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4x4 color_matrix : register(c0);

sampler texture_sampler;

float4 main(ps_input_t input) : COLOR {
    float4 sampler_color = tex2D(texture_sampler, input.uv);
    float4 transformed_color = mul(color_matrix, sampler_color);
    return float4(transformed_color.xyz, sampler_color.a);
}