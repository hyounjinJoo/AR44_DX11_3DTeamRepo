#pragma once
#include "define_GPU.h"
#include "IShader.h"

namespace mh
{
	using namespace mh::define;

	class ComputeShader : public IShader
	{
	public:
		ComputeShader(uint _threadGroupX, uint _threadGroupY, uint _threadGroupZ);
		ComputeShader();
		~ComputeShader();

		virtual eResult Load(const std::filesystem::path& _path) override;

		eResult CreateByCompile(const std::filesystem::path& _FullPath, const std::string_view _funcName);
		eResult CreateByHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize);
		eResult CreateByCSO(const std::filesystem::path& _FileName);

		void OnExcute();

		virtual void Binds();
		virtual void Clear();

	private:
		eResult CreateShader(const void* _pByteCode, size_t _ByteCodeSize);

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> mCSBlob;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> mCS;

		uint mThreadGroupCountX;
		uint mThreadGroupCountY;
		uint mThreadGroupCountZ;

		uint mGroupX;
		uint mGroupY;
		uint mGroupZ;
	};
}
