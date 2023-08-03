#ifndef SH_PARTICLE
#define SH_PARTICLE

#include "SH_Globals.hlsli"

struct tParticle
{
    float4 position;
    float4 direction;
    
    float lifeTime;
    float time;
    float speed;
    uint active;
};

struct tParticleShared
{
    uint gActiveCount;
};

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

#endif
