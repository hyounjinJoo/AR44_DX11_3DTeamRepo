#include "SH_Globals.hlsli"

struct VSIn
{
	float4 Position : POSITION;
};

struct VSOut
{
	float4 Position : SV_Position;
};


VSOut main(VSIn In)
{
	VSOut Out = (VSOut) 0.0f;
    
	float4 pos = mul(float4(In.Position), world);
	float4 viewPos = mul(float4(pos), view);
	float4 projPos = mul(float4(viewPos), projection);
	projPos.w = 1.0f;
    
	Out.Position = projPos;
    
	return Out;
}
