
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
		, mSkeleton{}
	{
	}

	Mesh::~Mesh()
	{
		for (size_t i = 0; i < mIndexInfos.size(); ++i)
		{
			//if (mIndexInfos[i].pIdxSysMem)
			//	delete mIndexInfos[i].pIdxSysMem;
		}
	}

	eResult Mesh::Save(const std::filesystem::path& _path)
	{
		std::fs::path filePath = PathMgr::GetContentPathRelative(GetResType());
		filePath /= _path;

		const std::fs::path& parentPath = filePath.parent_path();
		//폴더 없으면 폴더 생성
		if (false == std::fs::exists(parentPath))
		{
			std::fs::create_directories(parentPath);
		}

		filePath.replace_extension(define::strKey::Ext_Mesh);

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


	eResult Mesh::CreateFromContainer(const tFBXContainer* _fbxContainer)
	{
		if (nullptr == _fbxContainer)
			return eResult::Fail_Nullptr;

		//const tFBXContainer& container = _fbxContainer->GetContainer(0);

		UINT iVtxCount = (UINT)_fbxContainer->vecPos.size();
		std::vector<Vertex3D> vecVtx3d(iVtxCount);
		for (UINT i = 0; i < iVtxCount; ++i)
		{
			vecVtx3d[i].Pos = float4(_fbxContainer->vecPos[i], 1.f);
			vecVtx3d[i].UV = _fbxContainer->vecUV[i];
			vecVtx3d[i].Color = float4(1.f, 0.f, 1.f, 1.f);
			vecVtx3d[i].Normal = _fbxContainer->vecNormal[i];
			vecVtx3d[i].Tangent = _fbxContainer->vecTangent[i];
			vecVtx3d[i].BiNormal = _fbxContainer->vecBinormal[i];
			vecVtx3d[i].Weights = _fbxContainer->vecWeights[i];
			vecVtx3d[i].Indices = _fbxContainer->vecIndices[i];
		}
		//std::shared_ptr<Mesh> pMesh = std::make_shared<Mesh>();
		CreateVertexBuffer<Vertex3D>(vecVtx3d);


		// 인덱스 정보
		UINT iIdxBufferCount = (UINT)_fbxContainer->vecIdx.size();

		for (UINT i = 0; i < iIdxBufferCount; ++i)
		{
			CreateIndexBuffer(_fbxContainer->vecIdx[i]);
		}

		return eResult::Success;
	}
}


