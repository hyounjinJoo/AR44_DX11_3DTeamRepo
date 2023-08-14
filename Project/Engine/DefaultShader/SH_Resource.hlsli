#ifndef SH_RESOURCE
#define SH_RESOURCE

#include "SH_Common.hlsli"

//t 버퍼
REGISTER_DECLARE(Texture2D, albedoTexture, t, 0);
REGISTER_DECLARE(Texture2D, normalTexture, t, 1);
REGISTER_DECLARE(Texture2D, specularTexture, t, 2);
REGISTER_DECLARE(Texture2D, emissiveTexture, t, 3);

// Material Default Texture
//Texture2D albedoTexture : register(t0);
//Texture2D normalTexture : register(t1);
//Texture2D specularTexture : register(t2);
//Texture2D emissiveTexture : register(t3);


// defferd
REGISTER_DECLARE(Texture2D, positionTarget, t, 4);
REGISTER_DECLARE(Texture2D, normalTarget, t, 5);
REGISTER_DECLARE(Texture2D, albedoTarget, t, 6);
REGISTER_DECLARE(Texture2D, specularTarget, t, 7);
//Texture2D positionTarget : register(t4);
//Texture2D normalTarget : register(t5);
//Texture2D albedoTarget : register(t6);
//Texture2D specularTarget : register(t7);


// Light
REGISTER_DECLARE(Texture2D, diffuseLightTarget, t, 8);
REGISTER_DECLARE(Texture2D, specularLightTarget, t, 9);
//Texture2D diffuseLightTarget : register(t8);
//Texture2D specularLightTarget : register(t9);


//Atlas texture(Animation 2D)
REGISTER_DECLARE(Texture2D, atlasTexture, t, 12);
//Texture2D atlasTexture : register(t12);


// Light
//StructuredBuffer<LightAttribute> lightAttributes : register(t13);
//StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);

// Particle
//StructuredBuffer<Particle> particleBuffer : register(t15);
//RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
//RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u1);

// noise
//Texture2D NoiseTexture : register(t16);



// postProcess
Texture2D postProcessTexture : register(t60);
Texture2D guiGameTexture : register(t61);


//Texture2D 0 : register(t60);
//Texture2D 1 : register(t61);
//Texture2D 2 : register(t60);
//Texture2D 3 : register(t61);


#endif
