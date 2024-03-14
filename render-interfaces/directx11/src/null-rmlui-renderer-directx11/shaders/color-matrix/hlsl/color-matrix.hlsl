cbuffer constant_buffer : register(b0) {
    float4x4 color_matrix;
};

struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

sampler texture_sampler;
Texture2D texture_source;

float4 main(ps_input_t input) : SV_Target {
    float4 sampler_color = texture_source.Sample(texture_sampler, input.uv);
    float4 transformed_color = mul(color_matrix, sampler_color);
    return float4(transformed_color.xyz, sampler_color.a);
}