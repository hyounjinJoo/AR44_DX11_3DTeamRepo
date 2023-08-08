#include "SH_LightDir.hlsli"

		//albedo = Resources::Find<Texture>(L"PositionTarget");
		//lightMaterial->SetTexture(eTextureSlot::PositionTarget, albedo);
		//albedo = Resources::Find<Texture>(L"NormalTarget");
		//lightMaterial->SetTexture(eTextureSlot::NormalTarget, albedo);
		//albedo = Resources::Find<Texture>(L"SpecularTarget");
		//lightMaterial->SetTexture(eTextureSlot::SpecularTarget, albedo);

PS_OUT main(VSOut _in)
{
	PS_OUT output = (PS_OUT) 0.f;
    
	float2 vUV = _in.Position.xy / gfResolution;
	float4 vViewPos = positionTarget.Sample(anisotropicSampler, vUV);
    
	if (0.f == vViewPos.a)
		discard;
      
	float4 vViewNormal = normalTarget.Sample(anisotropicSampler, vUV);
        
	LightColor lightcolor = (LightColor) 0.f;
	CalculateLight3D(vViewPos.xyz, vViewNormal.xyz, 0, lightcolor);
    
	float SpecCoef = specularTarget.Sample(anisotropicSampler, vUV).x;
	float4 vSpec = decode(SpecCoef);

	output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
	output.vSpecular.xyz = lightcolor.specular.xyz; // * vSpec.xyz;
       
	output.vDiffuse.a = 1.f;
	output.vSpecular.a = 1.f;
    
	return output;
}
