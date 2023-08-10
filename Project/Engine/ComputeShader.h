#pragma once
#include "define_GPU.h"
#include "IShader.h"

namespace mh
{
	class ComputeShader : public IShader
	{
	public:
		ComputeShader(uint _threadGroupX, uint _threadGroupY, uint _threadGroupZ);
		ComputeShader();
		~ComputeShader();

		virtual define::eResult Load(const std::filesystem::path& _path) override;

		define::eResult CreateByCompile(const std::filesystem::path& _FullPath, const std::string_view _funcName);
		define::eResult CreateByHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize);
		define::eResult CreateByCSO(const std::filesystem::path& _FileName);

		void OnExcute();

		virtual void Binds();
		virtual void Clear();

	private:
		define::eResult CreateShader(const void* _pByteCode, size_t _ByteCodeSize);

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
