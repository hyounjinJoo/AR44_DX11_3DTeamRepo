#include "SH_Animation2D.hlsli"

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
  
    //color.a += time;
    if (CB_Animation2D.animationType == 1) // 2D
    {
		float2 vUV = In.UV;
        
        //애니메이션의 Left Top부터 Slice에 자신의 UV값을 곱해서 실제 UV값을 구해준다.
		float2 RealUV = CB_Animation2D.leftTop + CB_Animation2D.spriteSize * vUV;
        
		////텍스처 버퍼 8개 중 현재 그리고자 하는 텍스처 버퍼 번호를 받아와서 샘플링해준다.
		//int idx = Data.MTRL_SCALAR_STD2D_ANIM_TEXATLAS_IDX;
		color = AtlasTexture.Sample(anisotropicSampler, RealUV);
        
		//float2 diff = (CB_Animation2D.atlasSize - CB_Animation2D.spriteSize) / 2.0f;
		//float2 UV = (CB_Animation2D.leftTop - diff - CB_Animation2D.offset) + (CB_Animation2D.atlasSize * In.UV);
        
		//return float4(1.f, 1.f, 1.f, 1.f);
        
		//if (UV.x < CB_Animation2D.leftTop.x || UV.y < CB_Animation2D.leftTop.y 
  //          || UV.x > CB_Animation2D.leftTop.x + CB_Animation2D.spriteSize.x 
  //          || UV.y > CB_Animation2D.leftTop.y + CB_Animation2D.spriteSize.y)
  //          discard;
        
        
        
        //UV.x = -UV.x;
        //color = atlasTexture.Sample();
    }
    else 
    {
        //UV.x = -UV.x;
		color = float4(1.f, 1.f, 0.f, 1.f);
	}
    
    
    // noise, paper burn effect
    //if ( 0.0f < NoiseTime )
    //{
    //    color.a -= NoiseTexture.Sample(anisotropicSampler, In.UV).x * 0.25f * (10.0f - NoiseTime);
    //}
    
    
    if (color.a <= 0.0f)
        discard;
    
    //tLightColor lightColor = (tLightColor) 0.0f;
    //for (uint i = 0; i < CB_NumberOfLight.numberOfLight; i++)
    //{
    //    CalculateLight(lightColor, In.WorldPos.xyz, i);
    //}
    //color *= lightColor.diffuse;
    
    //color.a = 0.5f;
    //color = defaultTexture.Sample(anisotropicSampler, In.UV);
    return color;
}
