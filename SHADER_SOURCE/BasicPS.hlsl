#include "globals.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    
};

float4 main(VSOut In) : SV_Target
{
	return float4(1.0f, 0.0f, 1.0f, 1.0f);
}