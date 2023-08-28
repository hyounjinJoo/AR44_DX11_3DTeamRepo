#include "SH_Basic3D.hlsli"

//static float3 globalLightPos = float3(0.0f, 0.0f, 0.0f);
//static float3 globalLightDir = float3(1.0f, -1.0f, 1.0f);
//static float3 globalLightColor = float3(1.0f, 1.0f, 1.0f);
//static float3 globalLightAmb = float3(0.15f, 0.15f, 0.15f);

VSOut main(VSIn _in)
{
    VSOut _out = (VSOut)0.0f;

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
