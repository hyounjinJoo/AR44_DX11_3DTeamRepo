#include "SH_Basic3D.hlsli"

//diffuse
//specular
//ambient

//static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
//static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
//static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
//static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

// Material Default Texture
//Texture2D albedoTexture : register(t0);
//Texture2D normalTexture : register(t1);

float4 main(VSOut In) : SV_Target
{
	float4 OutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	if (CB_MaterialData.usedAlbedo == 1)
	{
		OutColor = albedoTexture.Sample(anisotropicSampler, In.UV);
	}

	float3 vNormal = In.ViewNormal;

	if (CB_MaterialData.usedNormal == 1)
	{
	// 물체의 표면에 적용될 탄젠트 공간 기준 방향벡터를 가져온다.
			vNormal = normalTexture.Sample(anisotropicSampler, In.UV).xyz;

	// 0~1값을 -1~1의 값으로 변환
			vNormal = (vNormal * 2.0f) - 1.0f;

			float3x3 matTBN =
			{
				In.ViewTangent,
				In.ViewBiNormal,
				In.ViewNormal,
			};

		vNormal = normalize(mul(vNormal, matTBN));
	}


	
	tLightColor lightColor = (tLightColor)0.0f;
	for (uint i = 0; i < CB_NumberOfLight.numberOfLight; i++)
	{
		
		CalculateLight3D(In.ViewPos, vNormal, i, lightColor);
	}

	OutColor.rgb = (OutColor.rgb * lightColor.diffuse.rgb
					+ lightColor.specular.rgb
					+ (OutColor.xyz * lightColor.ambient.rgb));


	return OutColor;
        
}
