#include "PCH_Engine.h"
#include "Com_Renderer_Mesh.h"
#include "GameObject.h"
#include "Com_Transform.h"
#include "Com_Animator2D.h"
#include "Com_Animator3D.h"
#include "json-cpp/json.h"
#include "ResMgr.h"

namespace mh
{
	Com_Renderer_Mesh::Com_Renderer_Mesh()
		: mMesh(nullptr)
		, mMaterials(1)
	{
	}

	Com_Renderer_Mesh::~Com_Renderer_Mesh()
	{
	}

	Com_Renderer_Mesh::Com_Renderer_Mesh(const Com_Renderer_Mesh& _other)
		: mMesh(_other.mMesh)
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

	void Com_Renderer_Mesh::Render()
	{
		//메쉬 또는 재질이 없을 경우 렌더링 불가능이므로 return;
		if (false == IsRenderReady())
			return;

		//애니메이션 확인
		IAnimator* Animator = static_cast<IAnimator*>(GetOwner()->GetComponent(define::eComponentType::Animator));
		if (Animator)
		{
			define::eDimensionType animDimType = Animator->GetDimensionType();

			Animator->BindData();

			if(define::eDimensionType::_3D == animDimType)
			{
				Com_Animator3D* Animator3D = static_cast<Com_Animator3D*>(Animator);

				UINT materialCount = GetMaterialCount();

				for (UINT i = 0; i < materialCount; ++i)
				{
					Material* mtrl = GetCurrentMaterial(i);
					if (mtrl)
					{
						mtrl->SetAnim3D(true);
						mtrl->SetBoneCount(Animator3D->GetBoneCount());
					}
				}
			}
		}
			
		
		//Render
		UINT iSubsetCount = GetMesh()->GetSubsetCount();
		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			if (nullptr != GetCurrentMaterial(i))
			{
				GetMesh()->BindBuffer(i);

				// 사용할 재질 업데이트
				GetCurrentMaterial(i)->Bind();

				// 사용할 메쉬 업데이트 및 렌더링
				GetMesh()->Render(i);
			}
		}

		if (Animator)
		{
			Animator->Clear();
		}
	}

	eResult Com_Renderer_Mesh::SaveJson(Json::Value* _pJson)
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

	eResult Com_Renderer_Mesh::LoadJson(const Json::Value* _pJson)
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



	void Com_Renderer_Mesh::SetMesh(const std::shared_ptr<Mesh> _mesh)
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
