#pragma once
#include "define_GPU.h"
#include "IRes.h"

namespace mh
{
	class Mesh : public IRes 
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual HRESULT Load(const std::filesystem::path& _path) override;

		template <typename Vertex>
		inline bool Create(const std::vector<Vertex>& _vecVtx, const std::vector<UINT>& _vecIdx);

		template <typename Vertex>
		inline bool CreateVertexBuffer(const std::vector<Vertex>& _vecVtx);
		bool CreateVertexBuffer(void* _data, size_t _dataStride, size_t _count);

		bool CreateIndexBuffer(void* _data, size_t _count);

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


	template<typename Vertex>
	inline bool Mesh::Create(const std::vector<Vertex>& _vecVtx, const std::vector<UINT>& _vecIdx)
	{
		if (false == CreateVertexBuffer((void*)_vecVtx.data(), sizeof(Vertex), _vecVtx.size()))
		{
			return false;
		}

		if (false == CreateIndexBuffer((void*)_vecIdx.data(), _vecIdx.size()))
		{
			return false;
		}
		
		return true;
	}
	template<typename Vertex>
	inline bool Mesh::CreateVertexBuffer(const std::vector<Vertex>& _vecVtx)
	{
		return CreateVertexBuffer((void*)_vecVtx.data(), sizeof(Vertex), _vecVtx.size());
	}
}
