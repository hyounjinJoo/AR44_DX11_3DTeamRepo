#ifndef SH_SAMPLER
#define SH_SAMPLER

SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

#endif
