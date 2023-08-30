#ifndef SH_USER_INTERFACE
#define SH_USER_INTERFACE

#ifndef __cplusplus
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

#endif
