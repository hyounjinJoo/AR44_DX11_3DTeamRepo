#ifndef SH_DEFFERED
#define SH_DEFFERED

#include "SH_Globals.hlsli"

struct VSIn
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
	float3 Tangent : TANGENT;
	float3 Normal : NORMAL;
	float3 BiNormal : BINORMAL;
};

struct VSOut
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;
    
	float3 ViewPos : POSITION;
    
	float3 ViewNormal : NORMAL;
	float3 ViewTanget : TANGENT;
	float3 ViewBiNormal : BINORMAL;
};

struct PSOut
{
	float4 Position : SV_Target0;
	float4 Normal : SV_Target1;
	float4 Albedo : SV_Target2;
	float4 Data : SV_Target3;
};

#endif
