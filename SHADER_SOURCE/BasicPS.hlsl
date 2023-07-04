#include "globals.hlsli"

struct VSIn
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
	float3 Normal : NORMAL;
};

struct VSOut
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;
	float3 WorldPos : POSITION;
	float3 Normal : NORMAL;
	float intensity : FOG;
};

//static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
//static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
//static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
//static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

float4 main(VSOut In) : SV_Target
{
	float4 OutColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    
	float4 Out = float4(1.0f, 0.0f, 1.0f, 1.0f);
        //OUT.intensity = saturate(dot(-LightDir, vWorldDir));
        //float3 vWorldDir = normalize(mul(float4(In.Position.xyz, 0.0f), world).xyz);
	float3 LightDir = normalize(globalLightDir);
	float intensity = saturate(dot(-LightDir, In.Normal));
    
	Out.xyz = (intensity * OutColor.xyz * globalLightColor)
                + (OutColor.xyz * globalLightAmb);
    
	return Out;
        
}