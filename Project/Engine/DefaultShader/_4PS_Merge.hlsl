#include "SH_Merge.hlsli"





//std::shared_ptr<Texture> pos = std::make_shared<Texture>();
//std::shared_ptr<Texture> normal = std::make_shared<Texture>();
//std::shared_ptr<Texture> albedo = std::make_shared<Texture>();
//std::shared_ptr<Texture> specular = std::make_shared<Texture>();



//Texture2D positionTexture : register(t20);
//Texture2D colorTexture : register(t21);

//Texture2D diffuseLightTarget : register(t6);
//Texture2D specularLightTarget : register(t7);

float4 main(VSOut In) : SV_Target
{
	float4 OutColor = (float4) 0.0f;
	float2 UV = In.Position.xy / CB_Global.fResolution;
    
	float4 viewPos = positionTarget.Sample(anisotropicSampler, UV);
	if (0.0f == viewPos.a)
		discard;
    
    
	OutColor = albedoTarget.Sample(anisotropicSampler, UV);
    
    
	float4 vDiffuse = diffuseLightTarget.Sample(anisotropicSampler, UV);
	float4 vSpecular = specularLightTarget.Sample(anisotropicSampler, UV);
    
	OutColor.rgb = (OutColor.rgb * vDiffuse.rgb) + vSpecular.rgb;
    
    
	return OutColor;
}
