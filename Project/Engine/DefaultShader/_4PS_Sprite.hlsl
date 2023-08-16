#include "SH_Globals.hlsli"
#include "SH_Func_Light.hlsli"
struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

//cbuffer time
//int 0

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    
    //color.a += time;
    
    if (CB_Animation2D.animationType == 1) // 2D
    {
		float2 diff = (CB_Animation2D.atlasSize - CB_Animation2D.spriteSize) / 2.0f;
		float2 UV = (CB_Animation2D.leftTop - diff - CB_Animation2D.offset) + (CB_Animation2D.atlasSize * In.UV);
        
		if (UV.x < CB_Animation2D.leftTop.x || UV.y < CB_Animation2D.leftTop.y 
            || UV.x > CB_Animation2D.leftTop.x + CB_Animation2D.spriteSize.x 
            || UV.y > CB_Animation2D.leftTop.y + CB_Animation2D.spriteSize.y)
            discard;
        
        //UV.x = -UV.x;
        color = atlasTexture.Sample(anisotropicSampler, UV);
    }
    else 
    {
        //UV.x = -UV.x;
        color = albedoTexture.Sample(anisotropicSampler, In.UV);
    }
    
    
    // noise, paper burn effect
    //if ( 0.0f < NoiseTime )
    //{
    //    color.a -= NoiseTexture.Sample(anisotropicSampler, In.UV).x * 0.25f * (10.0f - NoiseTime);
    //}
    
    
    if (color.a <= 0.0f)
        discard;
    
    tLightColor lightColor = (tLightColor) 0.0f;
    for (uint i = 0; i < CB_NumberOfLight.numberOfLight; i++)
    {
        CalculateLight(lightColor, In.WorldPos.xyz, i);
    }
    color *= lightColor.diffuse;
    
    //color.a = 0.5f;
    //color = defaultTexture.Sample(anisotropicSampler, In.UV);
    return color;
}
