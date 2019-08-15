cbuffer MatrixBuffer
{
    matrix world;
    matrix view;
    matrix proj;
};

struct VSOut
{
    float3 worldPos : Position;
    float4 color : Color;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float4 color : Color)
{
    VSOut output;

    output.worldPos = (float3)mul(float4(pos, 1.0f), mul(world, view));
    output.color = color;
    output.pos = mul(float4(pos, 1.0f), mul(world, mul(view, proj)));

    return output;
}