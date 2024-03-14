#define _blur_size 7
#define _num_weights ((_blur_size + 1) / 2)

cbuffer constant_buffer : register(b0) {
    float2 texel_offset;
};

struct vs_input_t {
    float2 position : POSITION0;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

struct ps_input_t {
    float4 position : SV_POSITION;
    float2 uv[_blur_size] : TEXCOORD0;
    float4 color : COLOR0;
};

ps_input_t main(vs_input_t input) {
    ps_input_t output;
    
    for(int i = 0; i < _blur_size; i++)
        output.uv[i] = input.uv - float(i - _num_weights + 1) * texel_offset;
    
    output.color = input.color;
    output.position = float4(input.position, 0.f, 1.f);
    return output;
}