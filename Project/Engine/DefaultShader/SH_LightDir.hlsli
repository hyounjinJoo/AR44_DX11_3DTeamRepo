#ifndef SH_LIGHTDIR
#define SH_LIGHTDIR
#include "SH_Globals.hlsli"
#include "SH_Func_Light.hlsli"

struct VSIn
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
};

struct VSOut
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;
};


struct PS_OUT
{
	float4 vDiffuse :	SV_Target0;
	float4 vSpecular :	SV_Target1;
};

#endif
