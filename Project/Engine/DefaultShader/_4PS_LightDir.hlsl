#include "SH_LightDir.hlsli"
#include "SH_Func_DecodeColor.hlsli"

PS_OUT main(VSOut _in)
{
	PS_OUT output = (PS_OUT) 0.f;
    
	float2 vUV = _in.Position.xy / CB_Global.fResolution;
	float4 vViewPos = positionTarget.Sample(anisotropicSampler, vUV);
    
	//xyz 셋다 0일 경우 버림
	if (false == any(vViewPos.xyz))
		discard;
      
	float4 vViewNormal = normalTarget.Sample(anisotropicSampler, vUV);
        
	tLightColor lightcolor = (tLightColor) 0.f;
	CalculateLight3D(vViewPos.xyz, vViewNormal.xyz, CB_NumberOfLight.indexOfLight, lightcolor);
    
	float SpecCoef = specularTarget.Sample(anisotropicSampler, vUV).x;
	float4 vSpec = DecodeColor(SpecCoef);

	output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
	output.vSpecular = lightcolor.specular; // * vSpec.xyz;
       
	output.vDiffuse.a = 1.f;
	output.vSpecular.a = 1.f;
    
	return output;
}
