#define _blur_size 7
#define _num_weights ((_blur_size + 1) / 2)

float2 texel_offset : register(c0);
float2 blur_texel_offset : register(c1);

struct vs_input_t {
    float2 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

struct ps_input_t {
    float4 position : POSITION0;
    float2 uv[_blur_size] : TEXCOORD0;
    float4 color : COLOR0;
};

ps_input_t main(vs_input_t input) {
    ps_input_t output;
    
    for(int i = 0; i < _blur_size; i++)
        output.uv[i] = input.uv - float(i - _num_weights + 1) * blur_texel_offset;
    
    output.color = input.color;
    output.position = float4(input.position, 0.0f, 1.0f);
    output.position.xy += texel_offset;
    return output;
}