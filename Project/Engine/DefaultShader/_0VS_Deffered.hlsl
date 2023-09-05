#include "SH_Deffered.hlsli"

VSOut main(VSIn _in)
{
	VSOut _out = (VSOut) 0.0f;
	
	tSkinningInfo info = (tSkinningInfo) 0;
	
	if(CB_MaterialData.bAnim)
	{
		info = Skinning(_in.Position.xyz, _in.Tangent, _in.BiNormal, _in.Normal, _in.vWeights, _in.vIndices);
	}
	else
	{
		info.Pos = _in.Position.xyz;
		info.Tangent = _in.Tangent;
		info.Binormal = _in.BiNormal;
		info.Normal = _in.Normal;
	}
    
	_out.Position = mul(float4(info.Pos, 1.f), CB_Transform.WVP);
	_out.ViewPos = mul(float4(info.Pos, 1.f), CB_Transform.WorldView).xyz;
	
	_out.ViewTangent =	normalize(mul(float4(info.Tangent.xyz, 0.0f), CB_Transform.WorldView).xyz);
	_out.ViewBiNormal =	normalize(mul(float4(info.Binormal.xyz, 0.0f), CB_Transform.WorldView).xyz);
	_out.ViewNormal =	normalize(mul(float4(info.Normal.xyz, 0.0f), CB_Transform.WorldView).xyz);
	
	_out.UV = _in.UV;
    
	return _out;
}

