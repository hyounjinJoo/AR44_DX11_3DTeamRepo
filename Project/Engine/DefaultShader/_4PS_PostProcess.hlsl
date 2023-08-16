#include "SH_PostProcess.hlsli"


float4 main(VSOut _in) : SV_Target
{
    float4 Color = (float4) 0.f;
        
    // VS_OUT 으로 전달한 SV_Position 값은 PixelShader 에 입력될 때 픽셀좌표로 변환해서 입력
    float2 UV = _in.Pos.xy / CB_Global.fResolution;
        
    float2 fAdd = float2(NoiseTexture.Sample(anisotropicSampler, _in.UV + CB_ParticleSystem.elapsedTime * 0.2f).x
                         , NoiseTexture.Sample(anisotropicSampler, _in.UV + float2(0.1f, 0.f) + CB_ParticleSystem.elapsedTime * 0.2f).x);
    fAdd -= fAdd / 2.f;
    UV += fAdd * 0.05f;
    
    Color = postProcessTexture.Sample(anisotropicSampler, UV);
    
    //Color.r *= 1.5f;


    //return float4(0.0f, 1.0f, 0.0f, 1.0f);
    return Color;
}
