#include "SH_LightDir.hlsli"


VSOut main(VSIn In)
{
	VSOut Out = (VSOut) 0.0f;
    
	Out.Position = float4(In.Position.xy * 2.0f, 0.0f, 1.0f);
    
	return Out;
}
