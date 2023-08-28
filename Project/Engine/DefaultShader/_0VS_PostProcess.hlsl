#include "SH_PostProcess.hlsli"


VSOut main(VSIn _in)
{
	VSOut _out = (VSOut) 0.f;
    
    //output.Pos = mul(float4(_in.Pos.xyz, 1.0f), world);
    
	_out.Pos = mul(_in.Pos, CB_Transform.WVP);
	_out.UV = _in.UV;
    
	return _out;
}
