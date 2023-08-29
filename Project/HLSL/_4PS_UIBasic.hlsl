#include "SH_UserInterFace.hlsli"

float4 main(VSOut In) : SV_Target
{
	float4 color = (float) 0.0f;
	color = Tex_0.Sample(anisotropicSampler, In.UV);
    
	return color;
}
