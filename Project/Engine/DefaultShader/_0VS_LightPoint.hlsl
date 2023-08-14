#include "SH_LightPoint.hlsli"


VSOut main(VSIn In)
{
	VSOut Out = (VSOut) 0.0f;
    
	float4 pos = mul(float4(In.Position), CB_Transform.world);
	float4 viewPos = mul(float4(pos), CB_Transform.view);
	float4 projPos = mul(float4(viewPos), CB_Transform.projection);
	projPos.w = 1.0f;
    
	Out.Position = projPos;
    
	return Out;
}
