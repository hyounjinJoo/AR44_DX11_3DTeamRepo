#ifndef SH_CONST_BUFFER
#define SH_CONST_BUFFER

cbuffer Global : register(b0)
{
	uint2  guResolution;
	float2 gfResolution;
	float  gDeltaTime;
}

cbuffer Transform : register(b1)
{
	row_major matrix world;
	row_major matrix inverseWorld;
	row_major matrix view;
	row_major matrix inverseView;
	row_major matrix projection;
}
cbuffer MaterialData : register(b2)
{
    uint usedAlbedo;
    uint usedNormal;
    uint padd1;
    uint padd2;
}

cbuffer Grid : register(b3)
{
    float4 cameraPosition;
    float2 cameraScale;
    float2 resolution;
}

cbuffer Animation : register(b4)
{
    float2 leftTop;
    float2 spriteSize;
    float2 offset;
    float2 atlasSize;

    uint animationType;
}

cbuffer NumberOfLight : register(b5)
{
	uint numberOfLight;
	uint indexOfLight;
}

cbuffer ParticleSystem : register(b6)
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
    int padding;
}

cbuffer Noise : register(b7)
{
	float4 NoiseSize;
	float NoiseTime;
}


#endif
