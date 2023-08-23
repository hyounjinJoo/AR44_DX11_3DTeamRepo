#include "PCH_Contents.h"
#include "Com_HWRenderer.h"

#include <Engine/StructBuffer.h>


namespace mh
{
	Com_HWRenderer::Com_HWRenderer()
		: mMySBuffer()
		, mExampleStructs()
	{
	}
	Com_HWRenderer::~Com_HWRenderer()
	{
	}
	void Com_HWRenderer::Init()
	{
		//구조화 버퍼 생성
		mMySBuffer = std::make_unique<StructBuffer>();

		//Descripttion 작성
		tSBufferDesc desc{};
		desc.REGISLOT_t_SRV = Register_t_ExampleSBuffer;//SBUFFER 선언하면 C++에서 지정된 번호를 쓸수있음
		desc.eSBufferType = eStructBufferType::READ_ONLY;//CPU에서 읽기 전용
		desc.TargetStageSRV = define::eShaderStageFlag::VS | define::eShaderStageFlag::PS;//정점 쉐이더와 픽셀 쉐이더에 데이터를 보낸다

		//설정값을 지정
		mMySBuffer->SetDesc(desc);

		//잘 전달되는지 보기 위해 값을 지정해준다(10개)
		for (int i = 0; i < 10; ++i)
		{
			ExampleStruct example{};
			example.example0 = i;
			example.example1 = i + 1;
			example.example2 = i + 2;
			example.example3 = i + 3;

			mExampleStructs.push_back(example);
		}

		//<사용할 구조체>, 구조화 버퍼의 최대 크기, 초기 데이터, 데이터의 갯수
		//여기선 지금 최대 크기와 데이터 갯수를 동일하게 해 놨음
		
		//초기 데이터가 있을 경우 생성과 동시에 데이터를 보낼 수 있음
		//mMySBuffer->Create<ExampleStruct>(mExampleStructs.size(), mExampleStructs.data(), mExampleStructs.size());

		//초기 데이터가 없을 경우에는 nullptr을 넣어준다
		mMySBuffer->Create<ExampleStruct>(mExampleStructs.size(), nullptr, (size_t)0);
		mMySBuffer->SetData(mExampleStructs.data(), mExampleStructs.size());

		//SRV에 데이터를 바인딩하면 데이터 연결은 완료
		mMySBuffer->BindDataSRV();
	}
}
