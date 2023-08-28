#include "SH_Deffered.hlsli"

VSOut main(VSIn _in)
{
	VSOut _out = (VSOut) 0.0f;
	
	if(CB_MaterialData.bAnim)
	{
		Skinning(_in.Position.xyz, _in.Tangent, _in.BiNormal, _in.Normal, _in.vWeights, _in.vIndices, 0);
	}
    
	_out.Position = mul(_in.Position, CB_Transform.WVP);
	_out.UV = _in.UV;
	
	float3 vViewTangent = normalize(mul(float4(_in.Tangent.xyz, 0.0f), CB_Transform.WorldView).xyz);
	float3 vViewBiNormal = normalize(mul(float4(_in.BiNormal.xyz, 0.0f), CB_Transform.WorldView).xyz);
	float3 vViewNormal = normalize(mul(float4(_in.Normal.xyz, 0.0f), CB_Transform.WorldView).xyz);

	_out.ViewTangent = vViewTangent.xyz;
	_out.ViewBiNormal = vViewBiNormal.xyz;
	_out.ViewNormal = vViewNormal.xyz;
	_out.ViewPos = mul(_in.Position, CB_Transform.WorldView).xyz;
    
	return _out;
}

