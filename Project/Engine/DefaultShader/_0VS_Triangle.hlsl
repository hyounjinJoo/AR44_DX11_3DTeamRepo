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

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
	Out.Pos = mul(In.Pos, CB_Transform.WVP);
	Out.UV = In.UV;
    
    return Out;
}
