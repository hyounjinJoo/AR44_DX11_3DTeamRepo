#ifndef SH_GRID
#define SH_GRID

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

#endif
