
#include "PCH_Engine.h"

#include "Mesh.h"
#include "RenderMgr.h"
#include "GPUMgr.h"
#include "FBXLoader.h"
#include "StructBuffer.h"

namespace mh
{
	Mesh::Mesh()
		: IRes(eResourceType::Mesh)
		, mVertexBuffer{}
		, mVBDesc{}
		, mVertexByteStride{}
		, mVertexCount{}
		//, mVertices{}
		, mIndexInfos{}
		, m_vecAnimClip{}
		, m_vecBones{}
		, m_pBoneFrameData{}
		, m_pBoneOffset{}
	{
	}

	Mesh::~Mesh()
	{
		for (size_t i = 0; i < mIndexInfos.size(); ++i)
		{
			//if (mIndexInfos[i].pIdxSysMem)
			//	delete mIndexInfos[i].pIdxSysMem;
		}

		if (m_pBoneFrameData)
			delete m_pBoneFrameData;

		if (m_pBoneOffset)
			delete m_pBoneOffset;
	}

	eResult Mesh::Load(const std::filesystem::path& _path)
	{
		return eResult::Fail_NotImplemented;
	}

	bool Mesh::CreateVertexBuffer(const void* _data, size_t _dataStride, size_t _count)
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

	bool Mesh::CreateIndexBuffer(const void* _data, size_t _count)
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

		//indexInfo.pIdxSysMem = new UINT[indexInfo.IdxCount];
		//memcpy(indexInfo.pIdxSysMem, _data, sizeof(UINT) * indexInfo.IdxCount);

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

	void Mesh::RenderAllMeshes() const
	{
		for (size_t i = 0; i < mIndexInfos.size(); ++i)
		{
			GPUMgr::Context()->DrawIndexed(mIndexInfos[i].IdxCount, 0, 0);
		}
	}
	
	void Mesh::RenderInstanced(UINT _subSet, UINT _instanceCount) const
	{
		GPUMgr::Context()->DrawIndexedInstanced(mIndexInfos[_subSet].IdxCount, _instanceCount, 0, 0, 0);
	}

	std::shared_ptr<Mesh> Mesh::CreateFromContainer(FBXLoader* _loader)
	{
		const tContainer* container = &(_loader->GetContainer(0));

		UINT iVtxCount = (UINT)container->vecPos.size();
		std::vector<Vertex3D> vecVtx3d(iVtxCount);
		for (UINT i = 0; i < iVtxCount; ++i)
		{
			vecVtx3d[i].Pos = float4(container->vecPos[i], 1.f);
			vecVtx3d[i].UV = container->vecUV[i];
			vecVtx3d[i].Color = float4(1.f, 0.f, 1.f, 1.f);
			vecVtx3d[i].Normal = container->vecNormal[i];
			vecVtx3d[i].Tangent = container->vecTangent[i];
			vecVtx3d[i].BiNormal = container->vecBinormal[i];
			vecVtx3d[i].Weights = container->vecWeights[i];
			vecVtx3d[i].Indices = container->vecIndices[i];
		}
		std::shared_ptr<Mesh> pMesh = std::make_shared<Mesh>();
		pMesh->CreateVertexBuffer<Vertex3D>(vecVtx3d);

		//D3D11_BUFFER_DESC tVtxDesc = {};

		//tVtxDesc.ByteWidth = sizeof(Vertex3D) * iVtxCount;
		//tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		//tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
		//if (D3D11_USAGE_DYNAMIC == tVtxDesc.Usage)
		//	tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		

		//mVertices.resize(iVtxCount);
		//tSub.pSysMem = malloc(tVtxDesc.ByteWidth);
		//Vertex3D* pSys = (Vertex3D*)tSub.pSysMem;

		//MH_ASSERT(pSys);


		//D3D11_SUBRESOURCE_DATA tSub = {};
		//tSub.pSysMem = vecVtx3d.data();

		//ComPtr<ID3D11Buffer> pVB = NULL;
		//if (FAILED(GPUMgr::Device()->CreateBuffer(&tVtxDesc, &tSub, pVB.GetAddressOf())))
		//{
		//	return NULL;
		//}


		//pMesh->mVertexBuffer = pVB;
		//pMesh->mVBDesc = tVtxDesc;
		////pMesh->mVertices = std::move(vecVtx3d);
		//pMesh->mVertexByteStride = sizeof(Vertex3D);
		//pMesh->mVertexCount = (UINT)vecVtx3d.size();

		

		//pMesh->mVertexSysMem = pSys;

		// 인덱스 정보
		
		//Info.IdxCount = (UINT)container->vecIdx.size();
		UINT iIdxBufferCount = (UINT)container->vecIdx.size();

		//D3D11_BUFFER_DESC tIdxDesc = {};

		
		for (UINT i = 0; i < iIdxBufferCount; ++i)
		{
			pMesh->CreateIndexBuffer(container->vecIdx[i]);

			//tIndexInfo Info{};
			//Info.tIBDesc.ByteWidth = (UINT)container->vecIdx[i].size() * sizeof(UINT); // Index Format 이 R32_UINT 이기 때문
			//Info.tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			//Info.tIBDesc.Usage = D3D11_USAGE_DEFAULT;


			////void* pSysMem = malloc(tIdxDesc.ByteWidth);
			////MH_ASSERT(nullptr != pSysMem);

			////memcpy(pSysMem, container->vecIdx[i].data(), tIdxDesc.ByteWidth);
			//D3D11_SUBRESOURCE_DATA tSub{};
			//tSub.pSysMem = pSysMem;

			//ComPtr<ID3D11Buffer> pIB = nullptr;
			//if (FAILED(GPUMgr::Device()->CreateBuffer(&tIdxDesc, &tSub, pIB.GetAddressOf())))
			//{
			//	return NULL;
			//}

			//tIndexInfo info = {};
			//info.tIBDesc = tIdxDesc;
			//info.IdxCount = (UINT)container->vecIdx[i].size();
			//info.pIdxSysMem = pSysMem;
			//info.IndexBuffer = pIB;

			//pMesh->mIndexInfos.push_back(info);
		}


		// Animation3D
		if (false == container->bAnimation)
			return pMesh;

		std::vector<tBone*>& vecBone = _loader->GetBones();
		UINT iFrameCount = 0;
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			tMTBone bone = {};
			bone.iDepth = vecBone[i]->iDepth;
			bone.iParentIndx = vecBone[i]->iParentIndx;
			bone.matBone = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i]->matBone);
			bone.matOffset = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i]->matOffset);
			bone.strBoneName = vecBone[i]->strBoneName;

			for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
			{
				tMTKeyFrame tKeyframe = {};
				tKeyframe.dTime = vecBone[i]->vecKeyFrame[j].dTime;
				tKeyframe.iFrame = j;
				tKeyframe.vTranslate.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[0];
				tKeyframe.vTranslate.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[1];
				tKeyframe.vTranslate.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[2];

				tKeyframe.vScale.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[0];
				tKeyframe.vScale.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[1];
				tKeyframe.vScale.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[2];

				tKeyframe.qRot.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[0];
				tKeyframe.qRot.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[1];
				tKeyframe.qRot.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[2];
				tKeyframe.qRot.w = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[3];

				bone.vecKeyFrame.push_back(tKeyframe);
			}

			iFrameCount = max(iFrameCount, (UINT)bone.vecKeyFrame.size());

			pMesh->m_vecBones.push_back(bone);
		}

		std::vector<tAnimClip*>& vecAnimClip = _loader->GetAnimClip();

		for (UINT i = 0; i < vecAnimClip.size(); ++i)
		{
			tMTAnimClip tClip = {};

			tClip.strAnimName = vecAnimClip[i]->strName;
			tClip.dStartTime = vecAnimClip[i]->tStartTime.GetSecondDouble();
			tClip.dEndTime = vecAnimClip[i]->tEndTime.GetSecondDouble();
			tClip.dTimeLength = tClip.dEndTime - tClip.dStartTime;

			tClip.iStartFrame = (int)vecAnimClip[i]->tStartTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.iEndFrame = (int)vecAnimClip[i]->tEndTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.iFrameLength = tClip.iEndFrame - tClip.iStartFrame;
			tClip.eMode = vecAnimClip[i]->eMode;

			pMesh->m_vecAnimClip.push_back(tClip);
		}

		// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
		if (pMesh->IsAnimMesh())
		{
			// BoneOffet 행렬
			std::vector<MATRIX> vecOffset;
			std::vector<tFrameTrans> vecFrameTrans;
			vecFrameTrans.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);

			for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
			{
				vecOffset.push_back(pMesh->m_vecBones[i].matOffset);

				for (size_t j = 0; j < pMesh->m_vecBones[i].vecKeyFrame.size(); ++j)
				{
					vecFrameTrans[(UINT)pMesh->m_vecBones.size() * j + i]
						= tFrameTrans{ float4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f)
						, float4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale, 0.f)
						, pMesh->m_vecBones[i].vecKeyFrame[j].qRot };
				}
			}


			pMesh->m_pBoneOffset = new StructBuffer;
			tSBufferDesc Desc{};
			Desc.eSBufferType = eStructBufferType::READ_ONLY;
			Desc.REGISLOT_t_SRV = Register_t_g_arrBoneMat;
			pMesh->m_pBoneOffset->SetDesc(Desc);
			pMesh->m_pBoneOffset->Create<MATRIX>(vecOffset.size(), vecOffset.data(), vecOffset.size());


			pMesh->m_pBoneFrameData = new StructBuffer;
			Desc = tSBufferDesc{};
			Desc.REGISLOT_t_SRV = Register_t_g_arrFrameTrans;
			Desc.eSBufferType = eStructBufferType::READ_ONLY;
			pMesh->m_pBoneFrameData->SetDesc(Desc);
			size_t SbufferSize = vecOffset.size() * (size_t)iFrameCount;
			pMesh->m_pBoneFrameData->Create<tFrameTrans>(SbufferSize, vecFrameTrans.data(), SbufferSize);
		}

		return pMesh;
	}
}


