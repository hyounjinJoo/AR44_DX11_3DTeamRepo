#include "PCH_Contents.h"
#include "Com_HWRenderer.h"

#include <Engine/StructBuffer.h>
#include <Engine/ResMgr.h>

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

		//<사용할 구조체>, 구조화 버퍼의 최대 크기, 초기 데이터, 데이터의 갯수
		//여기선 지금 최대 크기와 데이터 갯수를 동일하게 해 놨음
		
		//초기 데이터가 있을 경우 생성과 동시에 데이터를 보낼 수 있음
		//mMySBuffer->Create<ExampleStruct>(mExampleStructs.size(), mExampleStructs.data(), mExampleStructs.size());

		//초기 데이터가 없을 경우에는 nullptr을 넣어준다
		mMySBuffer->Create<ExampleStruct>(mExampleStructs.size(), nullptr, (size_t)0);


		//출력하고 싶은 메쉬를 찾아준다.
		//여기선 기본 리소스 중에 RectMesh를 사용할것이다.
		std::shared_ptr<Mesh> mesh = ResMgr::Find<Mesh>(strKey::Default::mesh::RectMesh);
		
		//없으면 에러띄워줌
		MH_ASSERT(mesh);

		//메쉬로 설정
		SetMesh(mesh);

	}
	void Com_HWRenderer::Update()
	{
		mExampleStructs.clear();

		srand((unsigned int)time(nullptr));
		
		//잘 전달되는지 보기 위해 0.f ~ 1.f 사이의 랜덤값을 지정해준다(10개)
		for (int i = 0; i < 10; ++i)
		{
			ExampleStruct example{};
			
			example.example0 = (float)(i * rand() % 100) / 100.f;
			example.example1 = (float)(i * rand() % 100) / 100.f;
			example.example2 = (float)(i * rand() % 100) / 100.f;
			example.example3 = (float)(i * rand() % 100) / 100.f;

			mExampleStructs.push_back(example);
		}

		mMySBuffer->SetData(mExampleStructs.data(), mExampleStructs.size());
	}
	void Com_HWRenderer::Render()
	{
		//SRV에 데이터를 바인딩하면 데이터 연결 완료
		mMySBuffer->BindDataSRV();

		Com_Renderer_Mesh::Render();

		mMySBuffer->UnBindData();
	}
}
