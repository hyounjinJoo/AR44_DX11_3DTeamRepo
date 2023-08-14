#ifndef SH_CONST_BUFFER
#define SH_CONST_BUFFER

#include "SH_Common.hlsli"

struct tCB_Global alignas(16)
{
	uint2 uResolution;
	float2 fResolution;
	float DeltaTime;
	int3  PadGlobal;
};



struct tCB_Transform alignas(16)
{
	MATRIX world;
	MATRIX inverseWorld;
	MATRIX view;
	MATRIX inverseView;
	MATRIX projection;
};




struct tCB_MaterialData alignas(16)
{
	float4 Diff;
	float4 Spec;
	float4 Amb;
	float4 Emv;

	BOOL usedAlbedo;
	BOOL usedNormal;
	BOOL usedSpecular;
	uint Pad1;
};



struct tCB_Grid alignas(16)
{
	float4 cameraPosition;
	float2 cameraScale;
	float2 resolution;
};


struct tCB_Animation2D alignas(16)
{
	float2 leftTop;
	float2 spriteSize;
	float2 offset;
	float2 atlasSize;

	uint animationType;
	float3 pad2;
};


struct tCB_NumberOfLight alignas(16)
{
	uint numberOfLight;
	uint indexOfLight;
	
	float2 pad3;
};




struct tCB_ParticleSystem alignas(16)
{
	float4 worldPosition;
	float4 startColor;
	float4 startSize;
    
	uint maxParticles;
	uint simulationSpace;
	float radius;
	float startSpeed;
    
	float startLifeTime;
	float deltaTime;
	float elapsedTime; //누적시간
	int LightPad;
};




struct tCB_Noise alignas(16)
{
	float4 NoiseSize;
	
	float NoiseTime;
	float3 NoisePad;
};





struct tCB_SBufferCount alignas(16)
{
	uint SBufferDataCount;
	uint3 SBufferPad;
};


CBUFFER(CB_Global, tCB_Global, 0);
CBUFFER(CB_Transform, tCB_Transform, 1);
CBUFFER(CB_MaterialData, tCB_MaterialData, 2);
CBUFFER(CB_Grid, tCB_Grid, 3);
CBUFFER(CB_Animation2D, tCB_Animation2D, 4);
CBUFFER(CB_NumberOfLight, tCB_NumberOfLight, 5);
CBUFFER(CB_ParticleSystem, tCB_ParticleSystem, 6);
CBUFFER(CB_Noise, tCB_Noise, 7);
CBUFFER(CB_SBufferCount, tCB_SBufferCount, 8);


#ifdef __cplusplus

namespace mh::define
{
	enum class eCBType
	{
		Global = Register_b_CB_Global,	
		Transform = Register_b_CB_Transform,	
		Material = Register_b_CB_MaterialData,	
		Grid = Register_b_CB_Grid,		
		Animation2D = Register_b_CB_Animation2D,	
		numberOfLight = Register_b_CB_NumberOfLight,
		ParticleSystem = Register_b_CB_ParticleSystem,
		Noise = Register_b_CB_Noise,	
		SBufferCount = Register_b_CB_SBufferCount,
		END
	};
};

#endif


#endif
