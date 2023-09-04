#ifndef SH_SKYBOX
#define SH_SKYBOX
#include "SH_Globals.hlsli"

#ifndef __cplusplus
#include "SH_Func_Light.hlsli"
struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;    
    float3 CUBEUV : TEXCOORD1;
};
#endif //__cplusplus
#endif
