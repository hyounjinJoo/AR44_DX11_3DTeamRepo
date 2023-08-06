#include "PCH_Engine.h"
#include "MeshData.h"

#include "define_Util.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "FBXLoader.h"

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
		stdfs::path fullPath = PathMgr::GetRelResourcePath(eResourceType::MeshData);
		fullPath /= _path;

		FBXLoader loader;
		loader.Init();
		loader.LoadFbx(fullPath);

		// 메쉬 가져오기
		std::shared_ptr<Mesh> pMesh = nullptr;
		pMesh = Mesh::CreateFromContainer(&loader);

		// ResMgr 에 메쉬 등록
		if (nullptr != pMesh)
		{
			std::string strMeshKey = "mesh\\";
			strMeshKey += fullPath.replace_extension(".mesh").string();
			
			ResMgr::Insert(strMeshKey, pMesh);

			// 메시를 실제 파일로 저장
			pMesh->Save(strMeshKey);
		}

		std::vector<std::shared_ptr<Material>> vecMtrl;

		// 메테리얼 가져오기
		for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
		{
			// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
			std::string strKey = StringConv::ConvertUnicodeToUTF8(loader.GetContainer(0).vecMtrl[i].strMtrlName);
			std::shared_ptr<Material> pMtrl = ResMgr::Find<Material>(strKey);
			MH_ASSERT(pMtrl.get());


			vecMtrl.push_back(pMtrl);
		}

		mMesh = pMesh;
		mMaterials = vecMtrl;

		return eResult::Success;
	}

	GameObject* MeshData::Instantiate()
	{
		return nullptr;
	}
}

