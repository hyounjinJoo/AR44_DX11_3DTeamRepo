#ifndef SH_LIGHTPOINT
#define SH_LIGHTPOINT
#include "SH_Globals.hlsli"
#include "SH_Func_Light.hlsli"
#include "SH_Func_DecodeColor.hlsli"

struct VSIn
{
	float4 Position : POSITION;
};

struct VSOut
{
	float4 Position : SV_Position;
};


struct PS_OUT
{
	float4 vDiffuse : SV_Target0;
	float4 vSpecular : SV_Target1;
};

#endif
