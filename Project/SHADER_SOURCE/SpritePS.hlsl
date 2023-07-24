#include "globals.hlsli"
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
    
    if (animationType == 1) // 2D
    {
        float2 diff = (atlasSize - spriteSize) / 2.0f;
        float2 UV = (leftTop - diff - offset) + (atlasSize * In.UV);
        
        if (UV.x < leftTop.x || UV.y < leftTop.y 
            || UV.x > leftTop.x + spriteSize.x 
            || UV.y > leftTop.y + spriteSize.y)
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
    
    LightColor lightColor = (LightColor) 0.0f;
    for (int i = 0; i < numberOfLight; i++)
    {
        CalculateLight(lightColor, In.WorldPos.xyz, i);
    }
    color *= lightColor.diffuse;
    
    //color.a = 0.5f;
    //color = defaultTexture.Sample(anisotropicSampler, In.UV);
    return color;
}
