cbuffer PostProcessingBuffer : register(b0)
{
    int mode;

    float param0;
    float param1;
    float param2;
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 0.0f;
    if (mode == 0) // none
    {
        finalColor = textureMap0.Sample(textureSampler, input.texCoord);
    }
    else if (mode == 1) // monochrome
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = (color.r + color.g + color.b) / 3.0f;
    }
    else if (mode == 2) // invert
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = 1.0f - color;
    }
    else if (mode == 3) // mirror (param0 = scaleX, param1 = scaleY, -1 flips, wrap sampler handles the rest)
    {
        float2 texCoord = input.texCoord * float2(param0, param1);
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    else if (mode == 4) // blur: 3x3 box (param0/param1 = uv offset per tap)
    {
        float u = input.texCoord.x;
        float v = input.texCoord.y;
        finalColor =
              textureMap0.Sample(textureSampler, float2(u, v))
            + textureMap0.Sample(textureSampler, float2(u - param0, v))
            + textureMap0.Sample(textureSampler, float2(u + param0, v))
            + textureMap0.Sample(textureSampler, float2(u, v - param1))
            + textureMap0.Sample(textureSampler, float2(u, v + param1))
            + textureMap0.Sample(textureSampler, float2(u - param0, v - param1))
            + textureMap0.Sample(textureSampler, float2(u + param0, v - param1))
            + textureMap0.Sample(textureSampler, float2(u - param0, v + param1))
            + textureMap0.Sample(textureSampler, float2(u + param0, v + param1));
        finalColor /= 9.0f; // average, not add, or the image blows out to white
    }
    else if (mode == 5) // combine2: lerp texture1 over texture0 (param0 = extra alpha)
    {
        float4 color0 = textureMap0.Sample(textureSampler, input.texCoord);
        float4 color1 = textureMap1.Sample(textureSampler, input.texCoord);
        float alpha = saturate(color1.a + param0);
        finalColor = lerp(color0, color1, alpha);
    }
    else if (mode == 6) // motion blur: not covered in class yet, passthrough
    {
        finalColor = textureMap0.Sample(textureSampler, input.texCoord);
    }
    else if (mode == 7) // chromatic aberration (param0/param1 = channel offset strength)
    {
        float2 distortion = float2(param0, -param1);
        float4 redChannel   = textureMap0.Sample(textureSampler, input.texCoord + distortion.x * input.texCoord);
        float4 greenChannel = textureMap0.Sample(textureSampler, input.texCoord);
        float4 blueChannel  = textureMap0.Sample(textureSampler, input.texCoord + distortion.y * input.texCoord);
        finalColor = float4(redChannel.r, greenChannel.g, blueChannel.b, 1.0f);
    }
    else if (mode == 8) // wave (param0 = amplitude in uv, param1 = number of waves across the screen)
    {
        float waveValue = input.texCoord.x * (3.14159f * param1);
        float2 texCoord = input.texCoord;
        texCoord.y += sin(waveValue) * param0;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    return finalColor;
}