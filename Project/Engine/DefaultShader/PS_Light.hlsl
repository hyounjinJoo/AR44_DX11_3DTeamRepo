#include "SH_Globals.hlsli"

struct VSIn
{
	float4 Position : POSITION;
};

struct VSOut
{
	float4 Position : SV_Position;
};

struct PSOut
{
	float4 Diffuse : SV_Target0;
	float4 Specular : SV_Target1;
};

//			Resources::Insert<Texture>(
//L"DiffuseTarget", diffuse);
//			Resources::Insert<Texture>(
//L"SpecualrTarget", specular);

//Texture2D albedoTarget : register(t2);
//Texture2D normalTarget : register(t3);
//Texture2D positionTarget : register(t4);
//Texture2D specularTarget : register(t5);

PSOut main(VSOut In)
{
	PSOut OutColor = (PSOut) 0.0f;
    
	float2 UV = In.Position.xy / float2(1600.0f, 900.0f);
	float4 viewPos = positionTarget.Sample(anisotropicSampler, UV);
    
	if (1.0f != viewPos.a)
		discard;
    
	float4 viewNormal = normalTarget.Sample(anisotropicSampler, UV);
    
	LightColor lightColor = (LightColor) 0.0f;
	CalculateLight3D(viewPos.xyz, viewNormal.xyz, 0, lightColor);
    
	float specCoef = specularTarget.Sample(anisotropicSampler, UV);
    
	OutColor.Diffuse = lightColor.diffuse + lightColor.ambient;
	OutColor.Specular = lightColor.specular * specCoef;
    
	OutColor.Diffuse.a = 1.0f;
	OutColor.Specular.a = 1.0f;
    
	return OutColor;
}
