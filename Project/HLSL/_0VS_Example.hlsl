#include "SH_ConstBuffer.hlsli"
#include "HW_Example.hlsli"

VSOut main(VSIn _in)
{
	VSOut _out = (VSOut) 0;
	
	_out.Color = _in.Color;
	
	_out.WorldPos = mul(_in.Pos, CB_Transform.world).xyz;
	_out.Pos = mul(_in.Pos, CB_Transform.WVP);
	_out.UV = _in.UV;
	
	return _out;
}
