#include "PCH_Engine.h"
#include "MeshData.h"

#include "PathMgr.h"

namespace mh
{
	namespace stdfs = std::filesystem;

	MeshData::MeshData()
		: IRes(define::eResourceType::MeshData)
	{
	}

	MeshData::~MeshData()
	{
	}

	eResult MeshData::Load(const std::filesystem::path& _path)
	{
		//stdfs::path fullPath = PathMgr::GetRelResourcePath(eResourceType::MeshData);
		//fullPath /= _path;

		//CFBXLoader loader;
		//loader.init();
		//loader.LoadFbx(strFullPath);

		//// 메쉬 가져오기
		//CMesh* pMesh = nullptr;
		//pMesh = CMesh::CreateFromContainer(loader);

		//// ResMgr 에 메쉬 등록
		//if (nullptr != pMesh)
		//{
		//	wstring strMeshKey = L"mesh\\";
		//	strMeshKey += path(strFullPath).stem();
		//	strMeshKey += L".mesh";
		//	CResMgr::GetInst()->AddRes<CMesh>(strMeshKey, pMesh);

		//	// 메시를 실제 파일로 저장
		//	pMesh->Save(strMeshKey);
		//}


		//vector<Ptr<CMaterial>> vecMtrl;

		//// 메테리얼 가져오기
		//for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
		//{
		//	// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
		//	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(0).vecMtrl[i].strMtrlName);
		//	assert(pMtrl.Get());

		//	vecMtrl.push_back(pMtrl);
		//}

		//CMeshData* pMeshData = new CMeshData(true);
		//pMeshData->m_pMesh = pMesh;
		//pMeshData->m_vecMtrl = vecMtrl;

		//return pMeshData;

		return eResult();
	}

	GameObject* MeshData::Instantiate()
	{
		return nullptr;
	}
}

