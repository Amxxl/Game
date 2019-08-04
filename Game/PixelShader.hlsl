Texture2D diffuseTexture : register(t0);
//Texture2D specularTexture;

SamplerState sampleType;

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

struct PixelInputType
{
    float4 position : SV_Position;
    float3 normal : Normal;
    float2 tex : TEXCOORD;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor = diffuseTexture.Sample(sampleType, input.tex);
    return textureColor;
}