cbuffer MatrixBuffer
{
    matrix wvp;
};

struct VSOut
{
    float3 worldPos : Position;
    float2 tex : Texcoord;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float2 tex : Texcoord)
{
    VSOut output;

    output.worldPos = (float3)mul(float4(pos, 1.0f), wvp);
    output.tex = tex;
    output.pos = mul(float4(pos, 1.0f), wvp);

    return output;
}