#include "SH_LightDir.hlsli"

//직사광선은 화면 전체를 덮으므로 
VSOut main(VSIn In)
{
	VSOut Out = (VSOut) 0.0f;
    
	Out.Position = float4(In.Position.xy * 2.0f, 0.0f, 1.0f);
	Out.UV = In.UV;
    
	return Out;
}
