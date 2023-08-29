#include "UIBasic.hlsli"

float4 main(VSOut In) : SV_Target
{
	float4 color = (float) 0.0f;
	
	
	//0.f ~ 1.f 사이로 제한
	float remain = saturate(CB_UniformData.UI_GAUGE_REMAIN);
	
	
	if(TRUE == CB_MaterialData.bTex_7)
	{
		float4 tempColor = Tex_7.Sample(anisotropicSampler, In.UV);
		if(0.f != tempColor.a)
			color += tempColor;
	}
	if (TRUE == CB_MaterialData.bTex_6)
	{
		float4 tempColor = Tex_6.Sample(anisotropicSampler, In.UV);
		if (0.f != tempColor.a)
			color += tempColor;
	}
	if (TRUE == CB_MaterialData.bTex_5)
	{
		float4 tempColor = Tex_5.Sample(anisotropicSampler, In.UV);
		if (0.f != tempColor.a)
			color += tempColor;
	}
	if (TRUE == CB_MaterialData.bTex_4)
	{
		float4 tempColor = Tex_4.Sample(anisotropicSampler, In.UV);
		if (0.f != tempColor.a)
			color += tempColor;
	}
	if (TRUE == CB_MaterialData.bTex_3)
	{
		float4 tempColor = Tex_3.Sample(anisotropicSampler, In.UV);
		if (0.f != tempColor.a)
			color += tempColor;
	}
	if (TRUE == CB_MaterialData.bTex_2)
	{
		float4 tempColor = Tex_2.Sample(anisotropicSampler, In.UV);
		if (0.f != tempColor.a)
			color += tempColor;
	}
	if (TRUE == CB_MaterialData.bTex_1)
	{
		float4 tempColor = Tex_1.Sample(anisotropicSampler, In.UV);
		if (0.f != tempColor.a)
			color += tempColor;
	}
	
	//마지막으로 게이지 텍스처를 샘플링(0번 텍스처는 필수 - 없을 경우 강제로 마젠타색 출력)
	if(false == CB_MaterialData.bTex_0)
	{
		return float4(1.f, 0.f, 1.f, 1.f);
	}
	else if (In.UV.x <= remain)
	{
		float4 tempColor = Tex_0.Sample(anisotropicSampler, In.UV);
		if (0.f != tempColor.a)
			color += tempColor;
	}

	//0~1 사이로 제한하고 반환
	color = saturate(color);
    
	return color;
}
