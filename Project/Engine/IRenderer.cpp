#include "PCH_Engine.h"
#include "IRenderer.h"

#include "Mesh.h"
#include "Material.h"

#include "GameObject.h"
#include "Com_Transform.h"
#include "Com_Animator2D.h"
#include "Com_Animator3D.h"
#include "json-cpp/json.h"
#include "ResMgr.h"
#include "Skeleton.h"

namespace mh
{
	IRenderer::IRenderer()
		: IComponent(define::eComponentType::Renderer)
		, mMesh(nullptr)
		, mMaterials(1)
	{
	}

	IRenderer::~IRenderer()
	{
	}

	IRenderer::IRenderer(const IRenderer& _other)
		: IComponent(_other)
		, mMesh(_other.mMesh)
		, mMaterials{}
	{
		mMaterials.resize(_other.mMaterials.size());
		for (size_t i = 0; i < mMaterials.size(); ++i)
		{
			//Shared Material은 공유 항목이므로 그대로 복사
			mMaterials[i].SharedMaterial = _other.mMaterials[i].SharedMaterial;

			//Dynamic Material은 고유 항목이므로 Clone해서 이쪽도 마찬가지로 고유 항목 생성
			if (_other.mMaterials[i].DynamicMaterial)
			{
				mMaterials[i].DynamicMaterial = std::unique_ptr<Material>(_other.mMaterials[i].DynamicMaterial->Clone());
			}
		}
	}

	eResult IRenderer::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = IComponent::SaveJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		Json::Value& jVal = *_pJson;

		//포인터의 경우 키값을 저장
		if (mMesh)
		{
			jVal[JSON_KEY(mMesh)] = mMesh->GetKey();
		}

		//TODO: FBX 로드를 위해 주석 처리
		//if (mMaterial)
		//{
		//	jVal[JSON_KEY(mMaterial)] = mMaterial->GetKey();
		//}

		return eResult::Success;
	}

	eResult IRenderer::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = IComponent::LoadJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		const Json::Value& jVal = *_pJson;

		if (jVal.isMember(JSON_KEY(mMesh)))
		{
			mMesh = ResMgr::Load<Mesh>(jVal[JSON_KEY(mMesh)].asString());
		}

		//TODO: FBX 로드를 위해 주석 처리
		//if (jVal.isMember(JSON_KEY(mMaterial)))
		//{
		//	mMaterial = ResMgr::Load<Material>(jVal[JSON_KEY(mMaterial)].asString());
		//}

		return eResult::Success;
	}



	void IRenderer::SetMesh(const std::shared_ptr<Mesh> _mesh)
	{
		mMesh = _mesh;

		//if (false == mMaterials.empty())
		//{
		//	mMaterials.clear();
		//	std::vector<tMaterialSet> materials;
		//	mMaterials.swap(materials);
		//}

		if (nullptr != mMesh)
			mMaterials.resize(mMesh->GetSubsetCount());
	}

}
