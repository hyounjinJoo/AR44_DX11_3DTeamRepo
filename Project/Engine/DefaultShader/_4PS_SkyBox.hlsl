#include "SH_SkyBox.hlsli"

float4 main(VSOut _in) : SV_Target
{
    float4 output = (float4) 0.0f;

    // sky Box Cube Texture에서 sampling
    //output = skyBoxCube.Sample(anisotropicSampler, _in.CUBEUV);
    
    return output;
}
