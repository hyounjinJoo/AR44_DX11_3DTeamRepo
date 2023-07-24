#ifndef SH_RESOURCE
#define SH_RESOURCE

// Material Default Texture
Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);

//Atlas texture
Texture2D atlasTexture : register(t12);

// Light
//StructuredBuffer<tLightAttribute> lightAttributes : register(t13);
//StructuredBuffer<tLightAttribute> lightAttributes3D : register(t14);

// tParticle
//StructuredBuffer<tParticle> particleBuffer : register(t15);
//RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
//RWStructuredBuffer<tParticleShared> ParticleSharedBuffer : register(u1);

// noise
//Texture2D NoiseTexture : register(t16);

// postProcess
Texture2D postProcessTexture : register(t60);
Texture2D guiGameTexture : register(t61);

#endif
