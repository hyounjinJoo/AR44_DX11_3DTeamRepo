#ifndef SH_POSTPROCESS
#define SH_POSTPROCESS

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

#endif
