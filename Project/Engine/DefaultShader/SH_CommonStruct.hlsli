#ifndef SH_COMMON_STRUCT
#define SH_COMMON_STRUCT
#include "SH_Common.hlsli"

//C++와 공동으로 사용하는 구조체 모음

struct alignas(16)  tCB_Global
{
	uint2 uResolution;
	float2 fResolution;
	float DeltaTime;
	int3 PadGlobal;
};

struct alignas(16) tCB_Transform
{
	MATRIX world;
	MATRIX inverseWorld;
	MATRIX view;
	MATRIX inverseView;
	MATRIX projection;
};


struct alignas(16) tCB_MaterialData
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



struct alignas(16)  tCB_Grid
{
	float4 cameraPosition;
	float2 cameraScale;
	float2 resolution;
};


struct alignas(16)  tCB_Animation2D
{
	float2 leftTop;
	float2 spriteSize;
	float2 offset;
	float2 atlasSize;

	uint animationType;
	float3 pad2;
};


struct alignas(16)  tCB_NumberOfLight
{
	uint numberOfLight;
	uint indexOfLight;
	
	float2 pad3;
};


struct alignas(16)  tCB_ParticleSystem
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




struct alignas(16)  tCB_Noise
{
	float4 NoiseSize;
	
	float NoiseTime;
	float3 NoisePad;
};


struct alignas(16) tCB_SBufferCount
{
	uint SBufferDataCount;
	uint3 SBufferPad;
};


struct alignas(16)  tLightColor
{
	float4 diffuse;
	float4 specular;
	float4 ambient;
};

struct alignas(16)  tLightAttribute
{
	tLightColor color;
	float4 position;
	float4 direction;
    
	int lightType;
	float radius;
	float angle;
	int padding;
};


#endif//SH_COMMON_STRUCT
