#include "SH_Grid.hlsli"


VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    float2 worldPos = float2(0.0f, 0.0f);
	worldPos.x = In.Pos.x * CB_Grid.cameraScale.x * CB_Grid.resolution.x + CB_Grid.cameraPosition.x;
	worldPos.y = In.Pos.y * CB_Grid.cameraScale.y * CB_Grid.resolution.y + CB_Grid.cameraPosition.y;
    
    const float meshScale = 2.0f;
    Out.Pos = float4(In.Pos.xy * meshScale, 0.999f, 1.0f);
    Out.UV = In.UV;
    Out.WorldPos = worldPos;
    
    return Out;
}
