cbuffer constant_buffer : register(b0) {
    matrix projection_matrix;
    float2 translation;
    float depth;
};

struct vs_input_t {
    float2 position : POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

ps_input_t main(vs_input_t input) {
    ps_input_t output;
    output.position = mul(projection_matrix, float4(input.position + translation, depth, 1.f));
    output.color = input.color;
    output.uv = input.uv;
    return output;
}