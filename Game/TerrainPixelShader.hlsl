Texture2D shaderTexture : register(t0);
Texture2D shaderTexture1 : register(t1);
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
    float4 textureColor;
    float4 textureColor1;
    float3 lightDir;
    float lightIntensity;
    float4 color;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    textureColor1 = shaderTexture1.Sample(SampleType, input.tex1);

    color = ambientColor;
    lightDir = -lightDirection;
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    if (lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
    }

    color = saturate(color);
    color = color * textureColor * textureColor1;

    // Combine the color map value into the texture color.
    //textureColor = saturate(textureColor * input.color);
    return color;
}
