#include "SH_UserInterFace.hlsli"
VSOut main(VSIn In)
{
	VSOut Out = (VSOut) 0.f;
    
	Out.Pos = mul(In.Pos, CB_Transform.WVP);
	Out.UV = In.UV;
    
	return Out;
}
