#pragma once
#include "define_GPU.h"
#include "IRes.h"

namespace mh
{
	using namespace mh;
	struct Vertex2D
	{
		float4 Pos;
		float4 Color;
		float2 UV;
	};
	struct Vertex3D
	{
		float4 Pos;
		float4 Color;
		float2 UV;
		float3 Tangent;
		float3 BiNormal;
		float3 Normal;
	};

	class Mesh : public IRes 
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual eResult Load(const std::filesystem::path& _path) override;

		template <typename Vertex>
		inline bool Create(const std::vector<Vertex>& _vecVtx, const std::vector<uint>& _vecIdx);

		template <typename Vertex>
		inline bool CreateVertexBuffer(const std::vector<Vertex>& _vecVtx);
		bool CreateVertexBuffer(void* _data, size_t _dataStride, size_t _count);

		bool CreateIndexBuffer(void* _data, size_t _count);

		void BindBuffer() const;
		void Render() const;
		void RenderInstanced(uint _count) const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		D3D11_BUFFER_DESC mVBDesc;
		uint mVertexByteStride;
		uint mVertexCount;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		D3D11_BUFFER_DESC mIBDesc;
		uint mIndexCount;
	};


	template<typename Vertex>
	inline bool Mesh::Create(const std::vector<Vertex>& _vecVtx, const std::vector<uint>& _vecIdx)
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
