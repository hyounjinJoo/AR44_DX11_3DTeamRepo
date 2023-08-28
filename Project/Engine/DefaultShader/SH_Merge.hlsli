#ifndef SH_MERGE
#define SH_MERGE
#include "SH_Globals.hlsli"

#ifndef __cplusplus
struct VSIn
{
	float4 Position : POSITION;
};

struct VSOut
{
	float4 Position : SV_Position;
};
#endif//__cplusplus

#endif
