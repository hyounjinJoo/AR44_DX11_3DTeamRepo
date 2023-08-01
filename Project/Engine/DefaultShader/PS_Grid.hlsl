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
    
    float2 WorldPos : POSITION;
};

float4 main(VSOut In) : SV_TARGET
{
    float4 Out = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    //if (In.Pos.x < 0)
    //    return;
    
	const uint width = 100;
	const uint height = 100;
    
	uint worldX = (uint) In.WorldPos.x;
	uint worldY = (uint) In.WorldPos.y;
    
    // 선 두께
    const uint thickness = 1u;
    
    if (((worldX + 1u) % width) <= thickness)
        return Out;
    
    if (((worldY + 1u) % height) <= thickness)
        return Out;
    
    discard;
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
}
