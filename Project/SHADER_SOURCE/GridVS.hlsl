#include "globals.hlsli"

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


VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    float2 worldPos = float2(0.0f, 0.0f);
    worldPos.x = In.Pos.x * cameraScale.x * resolution.x + cameraPosition.x;
    worldPos.y = In.Pos.y * cameraScale.y * resolution.y + cameraPosition.y;
    
    const float meshScale = 2.0f;
    Out.Pos = float4(In.Pos.xy * meshScale, 0.999f, 1.0f);
    Out.UV = In.UV;
    Out.WorldPos = worldPos;
    
    return Out;
}