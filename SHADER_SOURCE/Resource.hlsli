// Material Default Texture
Texture2D defaultTexture : register(t0);

//Atlas texture
Texture2D atlasTexture : register(t12);

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