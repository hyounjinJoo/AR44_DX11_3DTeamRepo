#include "SH_Globals.hlsli"

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

struct PSOut
{
	float4 Color : SV_Target0;
	float4 Normal : SV_Target1;
	float4 Position : SV_Target2;
	float4 Data : SV_Target3;
};


PSOut main(VSOut In) : SV_Target
{
	PSOut OutColor;
    
	float4 objColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
	if (usedAlbedo == 1)
	{
		objColor = albedoTexture.Sample(anisotropicSampler, In.UV);
	}
    
	float3 vNormal = In.ViewNormal;
    
	if (usedNormal == 1)
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
    
	OutColor.Color = objColor;
	OutColor.Normal = float4(vNormal, 1.0f);
	OutColor.Position = float4(In.ViewPos, 1.0f);
	OutColor.Data = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
	return OutColor;
}
