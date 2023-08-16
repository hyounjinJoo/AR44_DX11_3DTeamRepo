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
	
	MATRIX WorldView;
	MATRIX WVP;
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
	BOOL usedEmissive;
	
	// 3D Animation 정보
	BOOL bAnim;
	int BoneCount;
	int2 Padding_Material;
};

struct alignas(16)   tCB_ComputeShader
{
#ifdef __cplusplus
	const uint3 ThreadsPerGroup;
#else
	uint3 ThreadsPerGroup;
#endif
	
	uint Padding_CS1;
	
	uint3 NumGroup;
	uint Padding_CS2;
	
	uint3 TotalDataCount;
	uint Padding_CS3;
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
	float3 pad_Anim2D;
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

struct alignas(16)  tCB_Animation3D
{
	int BoneCount;
	int CurFrameIdx;
	int NextFrameIdx;
	float FrameRatio;
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



struct alignas(16) tFrameTrans
{
	float4 vTranslate;
	float4 vScale;
	float4 qRot;
};

struct alignas(16)  tSkinningInfo
{
	float3 vPos;
	float3 vTangent;
	float3 vBinormal;
	float3 vNormal;
};


#endif//SH_COMMON_STRUCT
