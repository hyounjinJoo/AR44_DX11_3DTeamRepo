#include "SH_Globals.hlsli"



float4 main( float4 pos : POSITION ) : SV_POSITION
{
	float4 worldPosition = mul(pos, CB_Transform.world);
	float4 viewPosition = mul(worldPosition, CB_Transform.view);
	float4 ProjPosition = mul(viewPosition, CB_Transform.projection);
    
    return ProjPosition;
}

