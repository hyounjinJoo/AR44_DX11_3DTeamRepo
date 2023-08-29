#include "UIBasic.hlsli"

float4 main(VSOut In) : SV_Target
{
	float4 color = (float) 0.0f;
	
	//0.f ~ 1.f 사이로 제한
	float remain = saturate(CB_UniformData.UI_GAUGE_REMAIN);
	
	if (remain < In.UV.x)
	{
		discard;
	}
		
		
	color = Tex_0.Sample(anisotropicSampler, In.UV);
    
	return color;
}
