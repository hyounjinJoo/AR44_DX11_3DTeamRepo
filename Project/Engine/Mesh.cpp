
#include "PCH_Engine.h"

#include "Mesh.h"
#include "RenderMgr.h"
#include "GPUMgr.h"
#include "FBXLoader.h"

namespace mh
{
	Mesh::Mesh()
		: IRes(eResourceType::Mesh)
		, mVBDesc{}
		, mVertexByteStride()
		, mVertexCount()
		, mIndexInfos{}
	{

	}

	Mesh::~Mesh()
	{
		for (size_t i = 0; i < mIndexInfos.size(); ++i)
		{
			if (mIndexInfos[i].pIdxSysMem)
				delete mIndexInfos[i].pIdxSysMem;
		}
	}

	eResult Mesh::Load(const std::filesystem::path& _path)
	{
		return eResult::Fail_NotImplemented;
	}

	bool Mesh::CreateVertexBuffer(void* _data, size_t _dataStride, size_t _count)
	{
		mVertexByteStride = (uint)_dataStride;
		mVertexCount = (uint)_count;

		// 버텍스 버퍼
		mVBDesc.ByteWidth = mVertexByteStride * mVertexCount;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mVBDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = _data;

		bool Result = SUCCEEDED(GPUMgr::Device()->CreateBuffer(&mVBDesc, &subData, mVertexBuffer.GetAddressOf()));

		if(false == Result)
		{
			mVertexBuffer = nullptr;
			mVBDesc = {};
			mVertexByteStride = 0u;
			mVertexCount = 0u;
		}
			
		return Result;
	}

	bool Mesh::CreateIndexBuffer(void* _data, size_t _count)
	{
		tIndexInfo indexInfo = {};
		indexInfo.IdxCount = (UINT)_count;
		indexInfo.tIBDesc.ByteWidth = (UINT)(sizeof(UINT) * _count);


		indexInfo.tIBDesc.CPUAccessFlags = 0;
		indexInfo.tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		indexInfo.tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		indexInfo.tIBDesc.MiscFlags = 0;
		indexInfo.tIBDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = _data;

		if (FAILED(GPUMgr::Device()->CreateBuffer(&indexInfo.tIBDesc, &subData, indexInfo.IndexBuffer.GetAddressOf())))
			return false;

		indexInfo.pIdxSysMem = new UINT[indexInfo.IdxCount];
		memcpy(indexInfo.pIdxSysMem, _data, sizeof(UINT) * indexInfo.IdxCount);

		mIndexInfos.push_back(indexInfo);
		return true;
	}

	void Mesh::BindBuffer(UINT _subSet) const
	{
		if ((UINT)mIndexInfos.size() <= _subSet)
			return;

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;
		GPUMgr::Context()->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &mVertexByteStride, &offset);
		GPUMgr::Context()->IASetIndexBuffer(mIndexInfos[_subSet].IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void Mesh::Render(UINT _subSet) const
	{
		GPUMgr::Context()->DrawIndexed(mIndexInfos[_subSet].IdxCount, 0, 0);
	}
	
	void Mesh::RenderInstanced(UINT _subSet, UINT _instanceCount) const
	{
		GPUMgr::Context()->DrawIndexedInstanced(mIndexInfos[_subSet].IdxCount, _instanceCount, 0, 0, 0);
	}

	std::shared_ptr<Mesh> Mesh::CreateFromContainer(FBXLoader* _loader)
	{
		const tContainer* container = &(_loader->GetContainer(0));

		UINT iVtxCount = (UINT)container->vecPos.size();

		D3D11_BUFFER_DESC tVtxDesc = {};

		tVtxDesc.ByteWidth = sizeof(Vertex3D) * iVtxCount;
		tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
		if (D3D11_USAGE_DYNAMIC == tVtxDesc.Usage)
			tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		std::vector<Vertex3D> vecVtx3d(iVtxCount);
		
		//mVertices.resize(iVtxCount);

		//tSub.pSysMem = malloc(tVtxDesc.ByteWidth);
		//Vertex3D* pSys = (Vertex3D*)tSub.pSysMem;

		//MH_ASSERT(pSys);
		for (UINT i = 0; i < iVtxCount; ++i)
		{
			vecVtx3d[i].Pos = float4(container->vecPos[i], 1.f);
			vecVtx3d[i].UV = container->vecUV[i];
			vecVtx3d[i].Color = float4(1.f, 0.f, 1.f, 1.f);
			vecVtx3d[i].Normal = container->vecNormal[i];
			vecVtx3d[i].Tangent = container->vecTangent[i];
			vecVtx3d[i].BiNormal = container->vecBinormal[i];
			//pSys[i].vWeights = container->vecWeights[i];
			//pSys[i].vIndices = container->vecIndices[i];
		}

		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = vecVtx3d.data();

		ComPtr<ID3D11Buffer> pVB = NULL;
		if (FAILED(GPUMgr::Device()->CreateBuffer(&tVtxDesc, &tSub, pVB.GetAddressOf())))
		{
			return NULL;
		}

		std::shared_ptr<Mesh> pMesh = std::make_shared<Mesh>();
		pMesh->mVertexBuffer = pVB;
		pMesh->mVBDesc = tVtxDesc;
		pMesh->mVertices = std::move(vecVtx3d);
		pMesh->mVertexByteStride = sizeof(Vertex3D);
		pMesh->mVertexCount = (UINT)pMesh->mVertices.size();

		//pMesh->mVertexSysMem = pSys;

		// 인덱스 정보
		UINT iIdxBufferCount = (UINT)container->vecIdx.size();
		D3D11_BUFFER_DESC tIdxDesc = {};

		for (UINT i = 0; i < iIdxBufferCount; ++i)
		{
			tIdxDesc.ByteWidth = (UINT)container->vecIdx[i].size() * sizeof(UINT); // Index Format 이 R32_UINT 이기 때문
			tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			tIdxDesc.Usage = D3D11_USAGE_DEFAULT;
			if (D3D11_USAGE_DYNAMIC == tIdxDesc.Usage)
				tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			void* pSysMem = malloc(tIdxDesc.ByteWidth);
			MH_ASSERT(nullptr != pSysMem);
			
			memcpy(pSysMem, container->vecIdx[i].data(), tIdxDesc.ByteWidth);
			tSub.pSysMem = pSysMem;

			ComPtr<ID3D11Buffer> pIB = nullptr;
			if (FAILED(GPUMgr::Device()->CreateBuffer(&tIdxDesc, &tSub, pIB.GetAddressOf())))
			{
				return NULL;
			}

			tIndexInfo info = {};
			info.tIBDesc = tIdxDesc;
			info.IdxCount = (UINT)container->vecIdx[i].size();
			info.pIdxSysMem = pSysMem;
			info.IndexBuffer = pIB;

			pMesh->mIndexInfos.push_back(info);
		}


		return pMesh;
	}
}
