#include "SH_Globals.hlsli"
struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    color = AlbedoTexture.Sample(anisotropicSampler, In.UV);
    
    //discard;
    
    return color;
}
