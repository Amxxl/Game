Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float4 tex : TEXCOORD;
};

float4 MD5ModelPixelShader(PixelInputType input) : SV_TARGET
{
    input.normal = normalize(input.normal);

    float4 textureColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    // Combine the color map value into the texture color.
    //textureColor = saturate(textureColor);

    //textureColor = saturate(textureColor * color);

    return textureColor;
}