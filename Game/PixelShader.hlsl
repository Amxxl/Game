cbuffer ObjectCBuf
{
    float3 materialColor;
    float specularIntensity;
    float specularPower;
};

struct PixelInputType
{
    float4 position : SV_Position;
    float3 normal : Normal;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}