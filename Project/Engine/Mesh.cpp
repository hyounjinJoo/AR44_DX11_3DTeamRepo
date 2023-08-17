
#include "PCH_Engine.h"

#include "Mesh.h"
#include "RenderMgr.h"
#include "GPUMgr.h"
#include "FBXLoader.h"
#include "StructBuffer.h"

//Save/Load 관련
#include "define_Util.h"
#include "PathMgr.h"
//#include "json-cpp/json.h"

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

	eResult Mesh::Save(const std::filesystem::path& _path)
	{
		std::fs::path filePath = PathMgr::GetContentPathRelative(GetResType());

		//폴더 없으면 폴더 생성
		if (false == std::fs::exists(filePath))
		{
			std::fs::create_directories(filePath);
		}

		// 파일 경로 만들기
		filePath /= _path;

		std::ofstream ofs(filePath, std::ios::binary);
		if (false == ofs.is_open())
		{
			return eResult::Fail_OpenFile;
		}
		
		//Key값 저장
		Binary::SaveStr(ofs, GetKey());

		//Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		//저장 필요 없음

		//D3D11_BUFFER_DESC mVBDesc;
		Binary::SaveValue(ofs, mVBDesc);
		
		//UINT mVertexByteStride;
		Binary::SaveValue(ofs, mVertexByteStride);
		//ofs << mVertexByteStride;

		//UINT mVertexCount;
		Binary::SaveValue(ofs, mVertexCount);
		//ofs << mVertexCount;

		//std::vector<unsigned char> mVertexSysMem;
		Binary::SaveValueVector(ofs, mVertexSysMem);

		//std::vector<tIndexInfo>		mIndexInfos;
		Binary::SaveValue(ofs, mIndexInfos.size());
		for (size_t i = 0; i < mIndexInfos.size(); ++i)
		{
			Binary::SaveValue(ofs, mIndexInfos[i].Val);
			Binary::SaveValueVector(ofs, mIndexInfos[i].IdxSysMem);
		}
		
		//// Animation3D 정보
		//std::vector<define::tMTAnimClip>		m_vecAnimClip;
		Binary::SaveValue(ofs, m_vecAnimClip.size());
		for (size_t i = 0; i < m_vecAnimClip.size(); ++i)
		{
			Binary::SaveStr(ofs, m_vecAnimClip[i].strAnimName);
			Binary::SaveValue(ofs, m_vecAnimClip[i].Val);
		}


		//std::vector<define::tMTBone>			m_vecBones;
		Binary::SaveValue(ofs, m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			Binary::SaveStr(ofs, m_vecBones[i].strBoneName);
			Binary::SaveValue(ofs, m_vecBones[i].Val);
			Binary::SaveValueVector(ofs, m_vecBones[i].vecKeyFrame);
		}

		//StructBuffer* m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
		//StructBuffer* m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)
		//저장 필요 X
		ofs.close();

		return eResult::Success;
	}

	eResult Mesh::Load(const std::filesystem::path& _path)
	{
		std::fs::path filePath = PathMgr::GetContentPathRelative(GetResType());

		//폴더 없으면 폴더 생성
		if (false == std::fs::exists(filePath))
		{
			std::fs::create_directories(filePath);
			ERROR_MESSAGE_W(L"파일이 없습니다.");
			return eResult::Fail_PathNotExist;
		}
		// 파일 경로 만들기
		filePath /= _path;

		std::ifstream ifs(filePath, std::ios::binary);
		if (false == ifs.is_open())
			return eResult::Fail_OpenFile;

		//Key값 가져오기
		std::string strKey;
		Binary::LoadStr(ifs, strKey);
		SetKey(strKey);

		////D3D11_BUFFER_DESC mVBDesc;
		//Binary::SaveValue(ofs, mVBDesc);

		////UINT mVertexByteStride;
		//ofs << mVertexByteStride;

		////UINT mVertexCount;
		//ofs << mVertexCount;

		//D3D11_BUFFER_DESC mVBDesc;
		Binary::LoadValue(ifs, mVBDesc);

		//UINT mVertexByteStride;
		Binary::LoadValue(ifs, mVertexByteStride);
		//ifs >> mVertexByteStride;

		//UINT mVertexCount;
		Binary::LoadValue(ifs, mVertexCount);
		//ifs >> mVertexCount;

		//std::vector<unsigned char> mVertexSysMem;
		Binary::LoadValueVector(ifs, mVertexSysMem);

		//Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		//버퍼 생성
		if (false == CreateVertexBuffer())
		{
			ERROR_MESSAGE_W(L"버텍스 버퍼 로드 실패");
			return eResult::Fail;
		}
			
		//std::vector<tIndexInfo>		mIndexInfos;
		{
			size_t size{};
			Binary::LoadValue(ifs, size);

			//매번 생성해서 집어넣음
			mIndexInfos.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				tIndexInfo info{};
				Binary::LoadValue(ifs, info.Val);
				Binary::LoadValueVector(ifs, info.IdxSysMem);
				mIndexInfos.push_back(info);
				if (false == CreateIndexBuffer())
				{
					ERROR_MESSAGE_W(L"인덱스 버퍼 로드 실패");
					return eResult::Fail;
				}
			}
		}


		//// Animation3D 정보
		//std::vector<define::tMTAnimClip>		m_vecAnimClip;
		{
			size_t size{};
			Binary::LoadValue(ifs, size);
			m_vecAnimClip.resize(size);
			for (size_t i = 0; i < size; ++i)
			{
				Binary::LoadStr(ifs, m_vecAnimClip[i].strAnimName);
				Binary::LoadValue(ifs, m_vecAnimClip[i].Val);
			}
		}

		//std::vector<define::tMTBone>			m_vecBones;
				//std::vector<define::tMTBone>			m_vecBones;
		{
			size_t size{};
			Binary::LoadValue(ifs, size);
			m_vecBones.resize(size);
			for (size_t i = 0; i < size; ++i)
			{
				Binary::LoadStr(ifs, m_vecBones[i].strBoneName);
				Binary::LoadValue(ifs, m_vecBones[i].Val);
				Binary::LoadValueVector(ifs, m_vecBones[i].vecKeyFrame);
			}
		}


		//StructBuffer* m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
		//StructBuffer* m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)


		return eResult::Success;
	}

	bool Mesh::CreateVertexBuffer(const void* _data, size_t _dataStride, size_t _dataCount)
	{
		if (nullptr == _data)
			return false;
		else if (false == SetVertexBufferData(_data, _dataStride, _dataCount))
			return false;
		

		return CreateVertexBuffer();
	}

	bool Mesh::SetVertexBufferData(const void* _data, size_t _dataStride, size_t _dataCount)
	{
		bool retVal = false;
		mVertexByteStride = (uint)_dataStride;
		mVertexCount = (uint)_dataCount;

		// 버텍스 버퍼
		mVBDesc.ByteWidth = mVertexByteStride * mVertexCount;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mVBDesc.CPUAccessFlags = 0;

		//정점 데이터를 memcpy를 통해서 복사
		size_t byteSize = (size_t)mVBDesc.ByteWidth;
		mVertexSysMem.resize(byteSize);
		if (0 == memcpy_s(mVertexSysMem.data(), byteSize, _data, byteSize))
			retVal = true;
		
		return retVal;
	}

	bool Mesh::SetIndexBufferData(const UINT* _data, size_t _dataCount)
	{
		bool result = false;

		tIndexInfo indexInfo = {};
		indexInfo.Val.IdxCount = (UINT)_dataCount;
		indexInfo.Val.tIBDesc.ByteWidth = (UINT)(sizeof(UINT) * _dataCount);
		indexInfo.Val.tIBDesc.CPUAccessFlags = 0;
		indexInfo.Val.tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		indexInfo.Val.tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		indexInfo.Val.tIBDesc.MiscFlags = 0;
		indexInfo.Val.tIBDesc.StructureByteStride = 0;

		//인덱스 버퍼 복사
		indexInfo.IdxSysMem.resize(_dataCount);
		if (0 == memcpy_s(indexInfo.IdxSysMem.data(), indexInfo.Val.tIBDesc.ByteWidth, _data, indexInfo.Val.tIBDesc.ByteWidth))
		{
			result = true;
			mIndexInfos.push_back(indexInfo);
		}

		return result;
	}

	bool Mesh::CreateVertexBuffer()
	{
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = mVertexSysMem.data();

		bool Result = SUCCEEDED(GPUMgr::Device()->CreateBuffer(&mVBDesc, &subData, mVertexBuffer.GetAddressOf()));

		if (false == Result)
		{
			mVertexBuffer = nullptr;
			mVBDesc = {};
			mVertexByteStride = 0u;
			mVertexCount = 0u;
		}

		return Result;
	}

	bool Mesh::CreateIndexBuffer()
	{
		bool result = false;
		tIndexInfo& indexInfo = mIndexInfos.back();

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = indexInfo.IdxSysMem.data();

		result = SUCCEEDED(GPUMgr::Device()->CreateBuffer(&indexInfo.Val.tIBDesc, &subData, indexInfo.IndexBuffer.GetAddressOf()));

		if (false == result)
		{
			mIndexInfos.pop_back();
		}

		return result;
	}



	bool Mesh::CreateIndexBuffer(const UINT* _data, size_t _dataCount)
	{
		if (nullptr == _data)
			return false;
		else if (false == SetIndexBufferData(_data, _dataCount))
			return false;

		return CreateIndexBuffer();
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
		GPUMgr::Context()->DrawIndexed(mIndexInfos[_subSet].Val.IdxCount, 0, 0);
	}

	void Mesh::RenderAllMeshes() const
	{
		for (size_t i = 0; i < mIndexInfos.size(); ++i)
		{
			GPUMgr::Context()->DrawIndexed(mIndexInfos[i].Val.IdxCount, 0, 0);
		}
	}
	
	void Mesh::RenderInstanced(UINT _subSet, UINT _instanceCount) const
	{
		GPUMgr::Context()->DrawIndexedInstanced(mIndexInfos[_subSet].Val.IdxCount, _instanceCount, 0, 0, 0);
	}


	std::shared_ptr<Mesh> Mesh::CreateFromContainer(FBXLoader& _loader)
	{
		const tContainer* container = &(_loader.GetContainer(0));

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

		std::vector<tBone*>& vecBone = _loader.GetBones();
		UINT iFrameCount = 0;
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			tMTBone bone = {};
			bone.Val.iDepth = vecBone[i]->iDepth;
			bone.Val.iParentIndx = vecBone[i]->iParentIndx;
			bone.Val.matBone = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i]->matBone);
			bone.Val.matOffset = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i]->matOffset);
			bone.strBoneName = vecBone[i]->strBoneName;

			for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
			{
				tMTKeyFrame Keyframe = {};
				Keyframe.dTime = vecBone[i]->vecKeyFrame[j].dTime;
				Keyframe.iFrame = j;
				
				const auto& keyFrameTransform = vecBone[i]->vecKeyFrame[j].matTransform.GetT();
				Keyframe.FrameTrans.vTranslate.x = (float)keyFrameTransform.mData[0];
				Keyframe.FrameTrans.vTranslate.y = (float)keyFrameTransform.mData[1];
				Keyframe.FrameTrans.vTranslate.z = (float)keyFrameTransform.mData[2];
				Keyframe.FrameTrans.vTranslate.w = (float)keyFrameTransform.mData[3];
				//float4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f),

				const auto& keyFrameScale = vecBone[i]->vecKeyFrame[j].matTransform.GetS();
				Keyframe.FrameTrans.vScale.x = (float)keyFrameScale.mData[0];
				Keyframe.FrameTrans.vScale.y = (float)keyFrameScale.mData[1];
				Keyframe.FrameTrans.vScale.z = (float)keyFrameScale.mData[2];
				Keyframe.FrameTrans.vScale.w = (float)keyFrameScale.mData[3];
				//float4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale, 0.f),

				const auto& keyFrameQuat = vecBone[i]->vecKeyFrame[j].matTransform.GetQ();
				Keyframe.FrameTrans.qRot.x = (float)keyFrameQuat.mData[0];
				Keyframe.FrameTrans.qRot.y = (float)keyFrameQuat.mData[1];
				Keyframe.FrameTrans.qRot.z = (float)keyFrameQuat.mData[2];
				Keyframe.FrameTrans.qRot.w = (float)keyFrameQuat.mData[3];

				bone.vecKeyFrame.push_back(Keyframe);
			}

			iFrameCount = max(iFrameCount, (UINT)bone.vecKeyFrame.size());

			pMesh->m_vecBones.push_back(bone);
		}

		std::vector<tAnimClip*>& vecAnimClip = _loader.GetAnimClip();

		for (UINT i = 0; i < vecAnimClip.size(); ++i)
		{
			tMTAnimClip tClip = {};

			tClip.strAnimName = vecAnimClip[i]->strName;
			tClip.Val.dStartTime = vecAnimClip[i]->tStartTime.GetSecondDouble();
			tClip.Val.dEndTime = vecAnimClip[i]->tEndTime.GetSecondDouble();
			tClip.Val.dTimeLength = tClip.Val.dEndTime - tClip.Val.dStartTime;
				 
			tClip.Val.iStartFrame = (int)vecAnimClip[i]->tStartTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.Val.iEndFrame = (int)vecAnimClip[i]->tEndTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.Val.iFrameLength = tClip.Val.iEndFrame - tClip.Val.iStartFrame;
			tClip.Val.eMode = vecAnimClip[i]->eMode;

			pMesh->m_vecAnimClip.push_back(tClip);
		}

		// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
		if (pMesh->IsAnimMesh())
		{
			// BoneOffet 행렬
			std::vector<MATRIX> vecOffset;
			std::vector<tFrameTranslation> vecFrameTrans;
			vecFrameTrans.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);

			for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
			{
				vecOffset.push_back(pMesh->m_vecBones[i].Val.matOffset);

				for (size_t j = 0; j < pMesh->m_vecBones[i].vecKeyFrame.size(); ++j)
				{
					vecFrameTrans[(UINT)pMesh->m_vecBones.size() * j + i]
						= pMesh->m_vecBones[i].vecKeyFrame[j].FrameTrans;
				}
			}

			//Create
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
			pMesh->m_pBoneFrameData->Create<tFrameTranslation>(SbufferSize, vecFrameTrans.data(), SbufferSize);
		}

		return pMesh;
	}
}


