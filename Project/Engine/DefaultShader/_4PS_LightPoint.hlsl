#include "SH_LightPoint.hlsli"

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
      
    // 광원 영역에 잡힌 position target의 위차값을 로컬영역으로 바꿔야한다.
    // 로컬 영역에서 광원메쉬 (spherer)의 내부에 있다면 실제로 point light 안에 들어가있다는 뜻
	float4 vLocalPos = mul(mul(vViewPos, inverseView), inverseWorld);
	if (length(vLocalPos.xyz) > 0.5f)
	{
		discard;
	}
    
	float4 vViewNormal = normalTarget.Sample(anisotropicSampler, vUV);
        
	LightColor lightcolor = (LightColor) 0.f;
	CalculateLight3D(vViewPos.xyz, vViewNormal.xyz, indexOfLight, lightcolor);
    
	float SpecCoef = specularTarget.Sample(anisotropicSampler, vUV).x;
	float4 vSpec = decode(SpecCoef);

	output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
	output.vSpecular.xyz = lightcolor.specular.xyz; // * vSpec.xyz;
       
	output.vDiffuse.a = 1.f;
	output.vSpecular.a = 1.f;
    
	return output;
}
