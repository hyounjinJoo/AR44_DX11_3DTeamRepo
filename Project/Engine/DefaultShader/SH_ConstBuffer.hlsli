#ifndef SH_CONST_BUFFER
#define SH_CONST_BUFFER

#include "SH_Common.hlsli"


REGISTER_DECLARE(cbuffer, CB_Global, b, 0)
{
	uint2	uResolution;
	float2	fResolution;
	float	DeltaTime;
	int		pad0;
};

REGISTER_DECLARE(cbuffer, CB_Transform, b, 1)
{
	MATRIX world;
	MATRIX inverseWorld;
	MATRIX view;
	MATRIX inverseView;
	MATRIX projection;
};

REGISTER_DECLARE(cbuffer, CB_MaterialData, b, 2)
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

REGISTER_DECLARE(cbuffer, CB_Grid, b, 3)
{
    float4 cameraPosition;
    float2 cameraScale;
    float2 resolution;
};

REGISTER_DECLARE(cbuffer, CB_Animation2D, b, 4)
{
	float2 leftTop;
	float2 spriteSize;
	float2 offset;
	float2 atlasSize;

	uint animationType;
	float3 pad2;
};

REGISTER_DECLARE(cbuffer, CB_NumberOfLight, b, 5)
{ 	
	uint numberOfLight;
	uint indexOfLight;
	
	float2 pad3;
};


REGISTER_DECLARE(cbuffer, CB_ParticleSystem, b, 6)
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

REGISTER_DECLARE(cbuffer, CB_Noise, b, 7)
{
	float4 NoiseSize;
	float NoiseTime;
	
	float3 NoisePad;
};

REGISTER_DECLARE(cbuffer, CB_SBufferCount, b, 8)
{
	uint SBufferDataCount;
	
	uint3 SBufferPad;
};


#ifdef __cplusplus

namespace mh::define
{
	enum class eCBType
	{
		Global = register_b_CB_Global,	
		Transform = register_b_CB_Transform,	
		Material = register_b_CB_MaterialData,	
		Grid = register_b_CB_Grid,		
		Animation2D = register_b_CB_Animation2D,	
		numberOfLight = register_b_CB_NumberOfLight,
		ParticleSystem = register_b_CB_ParticleSystem,
		Noise = register_b_CB_Noise,	
		SBufferCount = register_b_CB_SBufferCount,
		END
	};
};

#endif


#endif
