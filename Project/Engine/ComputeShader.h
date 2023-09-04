#pragma once
#include "define_GPU.h"
#include "IShader.h"


//1. BindData() 함수를 재정의해서 필요한 데이터를 바인딩하도록 설정
//2. Clear() 함수를 재정의해서 데이터 바인딩을 해제하는 함수도 재정의
//3. 데이터의 갯수 계산. 이건 처음에 한번만 계산해주면 될 때도 있고 매번 새로 계산해줘야할때도 있음.
//		매번 새로 계산해줘야 할 경우 BindData()에서 호출해주면 되고, 한번만 해주면 될 경우에는 생성자에서 해주면 됨
//4. 위의 3개 완료했으면 필요한 시점에 OnExcute() 호출

namespace mh
{
	class ComputeShader : public IShader
	{
	public:
		ComputeShader(uint3 _threadsPerGroup);
		virtual ~ComputeShader();

		virtual eResult Load(const std::filesystem::path& _filePath) override;

		eResult CreateByCompile(const std::filesystem::path& _FullPath, const std::string_view _funcName);
		eResult CreateByHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize);
		eResult CreateByCSO(const std::filesystem::path& _FileName);

		inline void CalculateGroupCount(const uint3& _dataCounts);

		void OnExcute();

	protected:
		virtual bool BindData() = 0;
		virtual void UnBindData() = 0;

		eResult CreateShader(const void* _pByteCode, size_t _ByteCodeSize);

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> mCSBlob;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> mCS;

		//스레드 갯수, 그룹 갯수 등 여기 저장
		tCB_ComputeShader mCB_ComputeShader;
	};

	void ComputeShader::CalculateGroupCount(const uint3& _dataCounts)
	{
		mCB_ComputeShader.TotalDataCount = _dataCounts;

		//필요한 그룹 수를 계산.
		mCB_ComputeShader.NumGroup.x = mCB_ComputeShader.TotalDataCount.x / mCB_ComputeShader.ThreadsPerGroup.x;
		if (0u == mCB_ComputeShader.NumGroup.x)
			mCB_ComputeShader.NumGroup.x = 1u;

		mCB_ComputeShader.NumGroup.y = mCB_ComputeShader.TotalDataCount.y / mCB_ComputeShader.ThreadsPerGroup.y;
		if (0u == mCB_ComputeShader.NumGroup.y)
			mCB_ComputeShader.NumGroup.y = 1u;

		mCB_ComputeShader.NumGroup.z = mCB_ComputeShader.TotalDataCount.z / mCB_ComputeShader.ThreadsPerGroup.z;
		if (0u == mCB_ComputeShader.NumGroup.z)
			mCB_ComputeShader.NumGroup.z = 1u;

		//mCB_ComputeShader.NumGroup.x = (UINT)ceilf((float)mCB_ComputeShader.TotalDataCount.x / (float)mCB_ComputeShader.ThreadsPerGroup.x);

		//mCB_ComputeShader.NumGroup.y = (UINT)ceilf((float)mCB_ComputeShader.TotalDataCount.y / (float)mCB_ComputeShader.ThreadsPerGroup.y);

		//mCB_ComputeShader.NumGroup.z = (UINT)ceilf((float)mCB_ComputeShader.TotalDataCount.z / (float)mCB_ComputeShader.ThreadsPerGroup.z);
	}
}
