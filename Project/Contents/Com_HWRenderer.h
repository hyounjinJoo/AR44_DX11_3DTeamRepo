#pragma once
#include <Engine/Com_Renderer_Mesh.h>

//구조체 정보가 들어있는 hlsl 헤더 참조
#include <HLSL/HW_Example.hlsli>

namespace mh
{
    //전방선언
    class StructBuffer;
    class Com_HWRenderer :
        public Com_Renderer_Mesh
    {
    public:
        Com_HWRenderer();
        virtual ~Com_HWRenderer();

        virtual void Init() override;
        virtual void Update() override;
        virtual void Render() override;
        virtual void RenderEnd() override;

    private:
        std::unique_ptr<StructBuffer> mMySBuffer;

        //HLSL에서 만든 구조체를 C++에서 똑같이 사용가능
        std::vector<ExampleStruct> mExampleStructs;
    };
}


