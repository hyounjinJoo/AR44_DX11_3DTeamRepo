#pragma once
#include "define_GPU.h"
#include "GameResource.h"

namespace mh
{
	class Mesh : public GameResource 
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual HRESULT Load(const std::filesystem::path& _path) override;

		bool CreateVertexBuffer(void* _data, UINT _count);
		bool CreateIndexBuffer(void* _data, UINT _count);

		void BindBuffer() const;
		void Render() const;
		void RenderInstanced(UINT _count) const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		D3D11_BUFFER_DESC mVBDesc;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		D3D11_BUFFER_DESC mIBDesc;
		UINT mIndexCount;
	};
}
