#include "SH_Particle.hlsli"


VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    Out.Pos = In.Pos;
    Out.iInstance = In.iInstance;
    
    return Out;
}
