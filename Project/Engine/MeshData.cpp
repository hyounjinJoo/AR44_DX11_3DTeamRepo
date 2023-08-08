#include "PCH_Engine.h"
#include "MeshData.h"

#include "define_Util.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "FBXLoader.h"
#include "GameObject.h"
#include "Com_Renderer_Mesh.h"

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
			stdfs::path strMeshKey = _path;
			strMeshKey.replace_extension(".fbx");
		
			ResMgr::Insert(strMeshKey.string(), pMesh);

			// 메시를 실제 파일로 저장
			//pMesh->Save(strMeshKey);
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
		GameObject* pNewObj = new GameObject;
		Com_Renderer_Mesh* Renderer = pNewObj->AddComponent<Com_Renderer_Mesh>();

		Renderer->SetMesh(mMesh);

		for (UINT i = 0; i < mMaterials.size(); ++i)
		{
			Renderer->SetMaterial(mMaterials[i], i);
		}

		//TODO: Animation 파트 추가
		//if (false == m_pMesh->IsAnimMesh())
		//	return pNewObj;

		//CAnimator3D* pAnimator = new CAnimator3D;
		//pNewObj->AddComponent(pAnimator);

		//pAnimator->SetBones(m_pMesh->GetBones());
		//pAnimator->SetAnimClip(m_pMesh->GetAnimClip());

		return pNewObj;
	}
}

