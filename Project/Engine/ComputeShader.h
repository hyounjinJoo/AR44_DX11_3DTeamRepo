#pragma once
#include "Graphics.h"
#include "GameResource.h"

namespace mh::graphics
{
	class ComputeShader : public GameResource
	{
	public:
		ComputeShader(UINT _threadGroupX, UINT _threadGroupY, UINT _threadGroupZ);
		ComputeShader();
		~ComputeShader();

		virtual HRESULT Load(const std::filesystem::path& _path) override;

		bool Create(const std::wstring& _file, const std::string& _funcName);
		void OnExcute();

		virtual void Binds();
		virtual void Clear();

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> mCSBlob;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> mCS;

		UINT mThreadGroupCountX;
		UINT mThreadGroupCountY;
		UINT mThreadGroupCountZ;

		UINT mGroupX;
		UINT mGroupY;
		UINT mGroupZ;
	};
}
