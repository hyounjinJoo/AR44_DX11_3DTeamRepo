#ifndef HW_EXAMPLE
#define HW_EXAMPLE

#include "SH_Common.hlsli"

struct alignas(16)  ExampleStruct
{
	float example0;
	float example1;
	float example2;
	float example3;
};

SBUFFER(ExampleSBuffer, ExampleStruct, t, 20);


//HLSL에서만 사용하고 싶을 경우
//#ifndef __cplusplus 를 정의해주면 된다
#ifndef __cplusplus

struct VSIn
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD;
};

struct VSOut
{
	float4 Pos : SV_Position;
	float3 WorldPos : POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD;
};

#endif

#endif
