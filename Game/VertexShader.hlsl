#include "Transform.hlsl"

struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float2 tc : Texcoord;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal, float2 tc : Texcoord)
{
    VSOut output;
    output.worldPos = (float3)mul(float4(pos, 1.0f), mul(worldMatrix, viewMatrix));
    output.normal = mul(mul(n, (float3x3)worldMatrix), (float3x3)viewMatrix);

    output.pos = mul(float4(pos, 1.0f), mul(worldMatrix, mul(viewMatrix, projectionMatrix)));
    output.tc = tc;

    return output;
}