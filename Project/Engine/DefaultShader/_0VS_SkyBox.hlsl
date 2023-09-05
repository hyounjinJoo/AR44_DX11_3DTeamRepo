#include "SH_SkyBox.hlsli"

VSOut main(VSIn _in)
{
    
    VSOut _out = (VSOut) 0.0f;
    
    // Sky Box는 이동하지 않아야 하므로 w 성분을 0.f로 하여 box를 카메라 시점에 고정시킨다.
    float4 viewPos = mul(float4(_in.Position.xyz, 0.0f), CB_Transform.view);
    // 원근 투영을 수행하기 위해서 w 성분을 1.f로 하여 투영행렬과 곱해준다.
    float4 projPos = mul(float4(viewPos.xyz, 1.0f), CB_Transform.projection);
    // z 좌표를 w로 설정하여 가장 마지막에 그려질 수 있도록 한다.
    projPos.z = projPos.w;
    
    // Pixel Shader에 전달될 값들을 설정
    _out.Position = projPos;
    _out.UV = _in.UV;
    // 큐브 맵에 사용될 uv를 보간처리하여 넘겨서 자연스럽게 Cube texture를 샘플링 할 수 있도록 한다.
    //_out.CUBEUV = normalize(_in.Position).xyz;
    
    return _out;
}
