#include "SH_Deffered.hlsli"


PSOut main(VSOut In) : SV_Target
{
	PSOut OutColor;
    
	
	//현재 시멘틱이 덮어씌워지고 있음.
	float4 objColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
	if (CB_MaterialData.usedAlbedo == 1)
	{
		objColor = albedoTexture.Sample(anisotropicSampler, In.UV);
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
			In.ViewTanget,
            In.ViewBiNormal,
            In.ViewNormal,
		};

		vNormal = normalize(mul(vNormal, matTBN));
	}
    
	OutColor.Position = float4(In.ViewPos, 1.0f);
	OutColor.Normal = float4(vNormal, 1.0f);
	OutColor.Albedo = objColor;
	OutColor.Data = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
	return OutColor;
}
