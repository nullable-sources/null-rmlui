float2 texel_offset : register(c0);
matrix projection_matrix : register(c1);
float2 translation : register(c5);
float depth : register(c6);

struct vs_input_t {
    float2 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

struct ps_input_t {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

ps_input_t main(vs_input_t input) {
    ps_input_t output;
    output.uv = input.uv;
    output.color = input.color;
    output.position = mul(projection_matrix, float4(input.position + translation, depth, 1.0f));
    output.position.xy += texel_offset;
    return output;
}