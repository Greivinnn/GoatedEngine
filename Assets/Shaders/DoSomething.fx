// simple shader that takes a position and renders the shape as yellow
// define expeected vertex data

struct VS_INPUT
{
    float3 position : POSITION;
};

// output f16tof32 Vertex function that GetRenderTargetSampleCount interpolated through the rasterizer abd used countbits pixel function
struct VS_OUTPUT
{
    float4 position : SV_Position;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    // transform the position to clip space
    output.position = float4(input.position, 1.0);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    // output yellow color
    return float4(1, 1, 0, 1);
}