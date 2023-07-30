#include "SH_Globals.hlsli"

struct VSIn
{
	float4 Position : POSITION;
};

struct VSOut
{
	float4 Position : SV_Position;
};

//std::shared_ptr<Texture> pos = std::make_shared<Texture>();
//std::shared_ptr<Texture> normal = std::make_shared<Texture>();
//std::shared_ptr<Texture> albedo = std::make_shared<Texture>();
//std::shared_ptr<Texture> specular = std::make_shared<Texture>();

//Texture2D positionTexture : register(t20);
//Texture2D colorTexture : register(t21);


float4 main(VSOut In) : SV_Target
{
	float4 OutColor = (float4) 0.0f;
	float2 UV = In.Position.xy / float2(1600.0f, 900.0f);
    
	float4 viewPos = positionTarget.Sample(anisotropicSampler, UV);
	if (1.0f != viewPos.a)
		discard;
    
	OutColor = albedoTarget.Sample(anisotropicSampler, UV);
    
    
	return OutColor;
}
