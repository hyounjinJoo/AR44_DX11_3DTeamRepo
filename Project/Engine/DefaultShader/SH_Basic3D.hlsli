#ifndef SH_BASIC3D
#define SH_BASIC3D
#include "SH_Globals.hlsli"
#include "SH_Func_Light.hlsli"

struct VSIn
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
	float3 Tangent : TANGENT;
	float3 BiNormal : BINORMAL;
	float3 Normal : NORMAL;
	
	
	float4 vWeights : BLENDWEIGHT;
	float4 vIndices : BLENDINDICES;
};

struct VSOut
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;
    
	float3 ViewPos : POSITION;
	float3 ViewTangent : TANGENT;
	float3 ViewBiNormal : BINORMAL;
	float3 ViewNormal : NORMAL;
};

#endif
