#include "SH_Deffered.hlsli"


PSOut main(VSOut _in) : SV_Target
{
	float4 vObjectColor = float4(0.4f, 0.4f, 0.4f, 1.f);
	float4 vOutColor = float4(1.f, 1.f, 1.f, 1.f);
	float3 vViewNormal = _in.ViewNormal;
    
	if (CB_MaterialData.usedAlbedo == 1)
	{
		vObjectColor = albedoTexture.Sample(anisotropicSampler, _in.UV);
	}
    
	if (CB_MaterialData.usedNormal == 1)
	{
		float3x3 matRot =
		{
			_in.ViewTanget,//X
			_in.ViewBiNormal,//Y
			_in.ViewNormal//Z
		};

		// 물체의 표면에 적용될 탄젠트 공간 기준 방향벡터를 가져온다.
		float3 vNormal = normalTexture.Sample(anisotropicSampler, _in.UV).xyz;
		
		//Normal 값은 0.f과 1.f 사이(0 ~ 255) -> -1 ~ 1 사이로 변환한다.
		// 0~1값을 -1~1의 값으로 변환 후 
		vNormal = normalize((vNormal * 2.f) - 1.f);
		
		//Normal 회전 행렬과 곱해서 실제 노말값을 구한다.
		vViewNormal = mul(vNormal, matRot);
	}
    
	
	PSOut OutColor = (PSOut) 0;
	
	OutColor.Albedo = vObjectColor;
	OutColor.Normal = float4(vViewNormal.xyz, 1.f);
	OutColor.Position = float4(_in.ViewPos.xyz, 1.f);
	//OutColor.Specular = float4
	OutColor.Emissive = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
	return OutColor;
}
