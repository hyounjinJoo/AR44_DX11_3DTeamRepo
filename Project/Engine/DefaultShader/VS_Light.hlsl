#include "SH_Globals.hlsli"


//Texture2D albedoTarget : register(t2);
//Texture2D normalTarget : register(t3);
//Texture2D positionTarget : register(t4);
//Texture2D specularTarget : register(t5);

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
    
	Out.Position = float4(In.Position.xy * 2.0f, 0.0f, 1.0f);
    
	return Out;
}
