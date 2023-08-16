#pragma once
#include "IRes.h"
#include "define_GPU.h"
#include "define_Struct.h"

namespace mh
{
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
		float3 Tangent;	//접선 벡터
		float3 Normal;	//법선 벡터
		float3 BiNormal;//종법선 벡터

		//Animation 가중치 및 인덱스
		float4 Weights;
		float4 Indices;
	};

	struct tIndexInfo
	{
		ComPtr<ID3D11Buffer>    IndexBuffer;

		struct Value
		{
			D3D11_BUFFER_DESC       tIBDesc;
			UINT				    IdxCount;
		} Val;

		std::vector<UINT>		IdxSysMem;
	};

	class FBXLoader;
	class StructBuffer;
	class Mesh : public IRes 
	{
	public:
		Mesh();
		virtual ~Mesh();

		static std::shared_ptr<Mesh> CreateFromContainer(FBXLoader* _loader);

		virtual eResult Save(const std::filesystem::path& _path) override;
		virtual eResult Load(const std::filesystem::path& _path) override;

		template <typename Vertex>
		inline bool Create(const std::vector<Vertex>& _vecVtx, const std::vector<uint>& _vecIdx);

		template <typename Vertex>
		inline bool CreateVertexBuffer(const std::vector<Vertex>& _vecVtx);
		bool CreateVertexBuffer(const void* _data, size_t _dataStride, size_t _dataCount);

		bool CreateIndexBuffer(const UINT* _data, size_t _dataCount);
		inline bool CreateIndexBuffer(const std::vector<UINT>& _indices);

		void BindBuffer(UINT _subSet = 0u) const;
		void Render(UINT _subSet = 0u) const;
		void RenderAllMeshes() const;
		
		void RenderInstanced(UINT _subSet, UINT _instanceCount) const;

		//const std::vector<Vertex3D> GetVertices() { return mVertices; }
		UINT GetSubsetCount() { return (UINT)mIndexInfos.size(); }


		//Animation 3D
		const std::vector<tMTBone>* GetBones() { return &m_vecBones; }
		UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
		const std::vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
		bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

		StructBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // 전체 본 프레임 정보
		StructBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // 각 뼈의 offset 행렬

	private:
		//버퍼를 만들지 않고 단순 데이터만 집어넣음
		bool SetVertexBufferData(const void* _data, size_t _dataStride, size_t _dataCount);
		bool SetIndexBufferData(const UINT* _data, size_t _dataCount);

		bool CreateVertexBuffer();
		bool CreateIndexBuffer();

		//void CreateAnimSBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		D3D11_BUFFER_DESC mVBDesc;
		UINT mVertexByteStride;
		UINT mVertexCount;
		std::vector<unsigned char> mVertexSysMem;
		
		std::vector<tIndexInfo>		mIndexInfos;

		// Animation3D 정보
		std::vector<define::tMTAnimClip>		m_vecAnimClip;
		std::vector<define::tMTBone>			m_vecBones;

		StructBuffer* m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
		StructBuffer* m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)
	};


	template<typename Vertex>
	inline bool Mesh::Create(const std::vector<Vertex>& _vecVtx, const std::vector<uint>& _vecIdx)
	{
		if (false == CreateVertexBuffer((void*)_vecVtx.data(), sizeof(Vertex), _vecVtx.size()))
		{
			return false;
		}

		if (false == CreateIndexBuffer((UINT*)_vecIdx.data(), _vecIdx.size()))
		{
			return false;
		}
		
		return true;
	}
	template<typename Vertex>
	inline bool Mesh::CreateVertexBuffer(const std::vector<Vertex>& _vecVtx)
	{
		return CreateVertexBuffer(static_cast<const void*>(_vecVtx.data()), sizeof(Vertex), _vecVtx.size());
	}
	inline bool Mesh::CreateIndexBuffer(const std::vector<UINT>& _indices)
	{
		return CreateIndexBuffer(static_cast<const UINT*>(_indices.data()), _indices.size());
	}

}
