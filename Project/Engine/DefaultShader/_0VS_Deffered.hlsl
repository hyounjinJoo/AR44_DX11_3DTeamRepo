#include "SH_Deffered.hlsli"

VSOut main(VSIn In)
{
	VSOut OUT = (VSOut) 0.0f;
    
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

