#ifndef SH_PARTICLE
#define SH_PARTICLE

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

#endif
