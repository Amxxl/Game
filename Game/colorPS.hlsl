
Texture2D tex;
SamplerState smp;

float4 main(float3 position : Position, float2 tc : Texcoord) : SV_TARGET
{
    return tex.Sample(smp, tc);
}