#include "SH_Animation2D.hlsli"

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
    float4 worldPosition = mul(In.Pos, CB_Transform.world);
	float4 viewPosition = mul(worldPosition, CB_Transform.view);
	float4 ProjPosition = mul(viewPosition, CB_Transform.projection);
    
    Out.WorldPos = worldPosition.xyz;
    Out.Pos = ProjPosition;
    Out.UV = In.UV;
    
    return Out;
}
