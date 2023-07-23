#include "globals.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;

    float3 ViewPos : POSITION;
    float3 ViewNormal : NORMAL;

    float3 ViewTanget : TANGENT;
    float3 ViewBiNormal : BINORMAL;
};

//static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
//static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
//static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
//static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

VSOut main(VSIn In)
{
    VSOut OUT = (VSOut)0.0f;

    float4 worldPosition = mul(In.Position, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);

    OUT.Position = ProjPosition;
    OUT.UV = In.UV;

    float3 vViewNormal = normalize(mul(float4(In.Normal.xyz, 0.0f), world).xyz);
    vViewNormal = normalize(mul(float4(vViewNormal, 0.0f), view).xyz);

    float3 vViewTangent = normalize(mul(float4(In.Tangent.xyz, 0.0f), world).xyz);
    vViewTangent = normalize(mul(float4(vViewTangent, 0.0f), view).xyz);

    float3 vViewBiNormal = normalize(mul(float4(In.BiNormal.xyz, 0.0f), world).xyz);
    vViewBiNormal = normalize(mul(float4(vViewBiNormal, 0.0f), view).xyz);

    OUT.ViewPos = viewPosition.xyz;
    OUT.ViewNormal = vViewNormal.xyz;
    OUT.ViewTanget = vViewTangent.xyz;
    OUT.ViewBiNormal = vViewBiNormal.xyz;

    return OUT;
}
