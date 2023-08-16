#include "SH_Globals.hlsli"
struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
    float4 worldPosition = mul(In.Pos, CB_Transform.world);
	float4 viewPosition = mul(worldPosition, CB_Transform.view);
	float4 ProjPosition = mul(viewPosition, CB_Transform.projection);
    
    Out.WorldPos = worldPosition.xyz;
    Out.Pos = ProjPosition;
    Out.Color = In.Color;
    Out.UV = In.UV;
    
    return Out;
}
