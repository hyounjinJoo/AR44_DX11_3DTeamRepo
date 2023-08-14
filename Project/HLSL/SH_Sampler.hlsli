#ifndef SH_SAMPLER
#define SH_SAMPLER
#include "SH_COMMON.hlsli"

SAMPLER(pointSampler, s, 0);
SAMPLER(linearSampler, s, 1);
SAMPLER(anisotropicSampler, s, 2);

#endif
