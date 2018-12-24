Texture2D alphaTexture : register(t0);
Texture2D diffuseTexture1 : register(t1);
Texture2D diffuseTexture2 : register(t2);
Texture2D diffuseTexture3 : register(t3);
Texture2D diffuseTexture4 : register(t4);

SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
    float2 tex1 : TEXCOORD1;
};

float4 TerrainPixelShader(PixelInputType input) : SV_TARGET
{
    float4 alphaMap;
    float4 textureColor1;
    float4 textureColor2;
    float4 textureColor3;
    float4 textureColor4;
    float4 baseColor;
    float4 color;

    // Sample the pixel color from the texture using the sampler at first set of texture coordinates.
    textureColor1 = diffuseTexture1.Sample(SampleType, input.tex);
    textureColor2 = diffuseTexture2.Sample(SampleType, input.tex);
    textureColor3 = diffuseTexture3.Sample(SampleType, input.tex);
    textureColor4 = diffuseTexture4.Sample(SampleType, input.tex);

    // Sample the alpha map using second set of texture coordinates.
    alphaMap = alphaTexture.Sample(SampleType, input.tex1);

    // Set the base color to the first color texture.
    baseColor = textureColor1;

    color = lerp(baseColor, textureColor2, alphaMap.r);
    color = lerp(color, textureColor3, alphaMap.g);
    color = lerp(color, textureColor4, alphaMap.b);

    return color;
}
