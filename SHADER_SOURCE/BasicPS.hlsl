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
    
	float3 ViewPos : POSITION;
	float3 ViewNormal : NORMAL;
	float intensity : FOG;
};

//diffuse
//specular
//ambient

//static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
//static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
//static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
//static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

float4 main(VSOut In) : SV_Target
{
	float4 OutColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
    

	LightColor lightColor = (LightColor)0.0f;


	//void CalculateLight3D ( float3 viewPos, float3 viewNormal, int lightIdx, inout LightColor lightColor)
	for (int i = 0; i < numberOfLight; i++)
	{
		CalculateLight3D(In.ViewPos, In.ViewNormal, i, lightColor);
	}

	OutColor.rgb = (OutColor.rgb * lightColor.diffuse.rgb
					+ lightColor.specular.rgb
					+ (OutColor.xyz * lightColor.ambient.rgb));

	//LightAttribute lightAttribute = lightAttributes[0];


	////광원의 방향을 월드 좌표계에서 뷰 좌표계로 변환
	//float3 ViewLightDir = normalize(mul(float4(lightAttribute.direction.xyz, 0.0f), view));

	//float intensity = saturate(dot(-ViewLightDir, In.ViewNormal));
	//float fSpecPow = 0.0f;

	////뷰 스페이스 상에서 표면의 빛의 세기를 구해야함
	//float3 vViewReflect
	//= normalize(ViewLightDir + 2.0f * dot(-ViewLightDir, In.ViewNormal) * In.ViewNormal);

	//// 시점에서 표면을 향하는 벡터
	//float3 vEye = normalize(In.ViewPos);

	////시선 벡터랑 반사벡터를 내적해서 반사광의 세기를 구한다.
	//fSpecPow = saturate(dot(-vEye, vViewReflect));
	//fSpecPow = pow(fSpecPow, 30);


	//OutColor.rgb = (OutColor.rgb * lightAttribute.color.diffuse.rgb * intensity
	//                + lightAttribute.color.specular.rgb * fSpecPow * 1.0f
	//                + (OutColor.xyz * lightAttribute.color.ambient.rgb));
    
	return OutColor;
        
}