#ifndef SH_PARTICLE
#define SH_PARTICLE
#include "SH_Globals.hlsli"

struct alignas(16)   tParticle
{
	float4 position;
	float4 direction;
    
	float lifeTime;
	float time;
	float speed;
	uint active;
};

struct alignas(16)    tParticleShared
{
	uint gActiveCount;
};

// Particle
//StructuredBuffer<Particle> particleBuffer : register(t15);
//RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
//RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u1);
SBUFFER(ParticleBuffer, tParticle, t, 15);
SBUFFER_RW(RW_ParticleBuffer, tParticle, u, 0);
SBUFFER_RW(RW_SharedParticleBuffer, tParticleShared, u, 1);

//HLSL
#ifndef __cplusplus
struct VSIn
{
	float4 Pos : POSITION;
	uint iInstance : SV_InstanceID;
};

struct GSOutput
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	uint iInstance : SV_InstanceID;
};

struct VSOut
{
	float4 Pos : SV_Position;
	uint iInstance : SV_InstanceID;
};
#endif//HLSL
#endif
