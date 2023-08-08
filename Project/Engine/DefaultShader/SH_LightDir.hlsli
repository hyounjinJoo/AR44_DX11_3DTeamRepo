#ifndef SH_LIGHTDIR
#define SH_LIGHTDIR

#include "SH_Globals.hlsli"

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
	float4 vDiffuse : SV_Target;
	float4 vSpecular : SV_Target1;
};

#endif
