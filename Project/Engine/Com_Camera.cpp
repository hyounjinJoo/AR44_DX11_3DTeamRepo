
#include "PCH_Engine.h"

#include "Com_Camera.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "Application.h"
#include "RenderMgr.h"
#include "IScene.h"
#include "SceneMgr.h"
#include "Material.h"
#include "IRenderer.h"
#include "SceneMgr.h"
#include "ResMgr.h"
#include "Com_Light3D.h"

#include "MultiRenderTarget.h"

extern mh::Application gApplication;

namespace mh
{
	using namespace mh::define;

	MATRIX Com_Camera::gView = MATRIX::Identity;
	MATRIX Com_Camera::gInverseView = MATRIX::Identity;
	MATRIX Com_Camera::gProjection = MATRIX::Identity;

	Com_Camera::Com_Camera()
		: IComponent(define::eComponentType::Camera)
		, mProjType(eProjectionType::None)
		, mAspectRatio(1.0f)
		, mNear(1.0f)
		, mFar(50000.f)
		, mScale(1.0f)
	{
		EnableLayerMasks();
	}

	Com_Camera::~Com_Camera()
	{
	}

	void Com_Camera::Init()
	{
		//RegisterCameraInRenderer();
	}

	void Com_Camera::Update()
	{
		
	}

	void Com_Camera::FixedUpdate()
	{
		if (eProjectionType::None == mProjType)
		{
			ERROR_MESSAGE_W(L"카메라의 투영행렬 타입을 설정하지 않았습니다.");
			MH_ASSERT(false);
			return;
		}

		CreateViewMatrix();

		RegisterCameraInRenderer();
	}

	void Com_Camera::RenderCamera()
	{
		gView = mView;
		gInverseView = mView.Invert();
		gProjection = mProjection;

		SortGameObjects();

		//deffered opaque render
		RenderMgr::GetMultiRenderTarget(eMRTType::Deffered)->Bind();
		RenderDeffered();

		//// deffered light 
		RenderMgr::GetMultiRenderTarget(eMRTType::Light)->Bind();
		// 여러개의 모든 빛을 미리 한장의 텍스처에다가 계산을 해두고
		// 붙여버리자

		const auto& Lights = RenderMgr::GetLights();
		for (size_t i = 0; i < Lights.size(); ++i)
		{
			Lights[i]->Render();
		}

		// Forward render
		RenderMgr::GetMultiRenderTarget(eMRTType::Swapchain)->Bind();


		//// defferd + swapchain merge
		std::shared_ptr<Material> mergeMaterial = ResMgr::Find<Material>(strKey::Default::material::MergeMaterial);
		std::shared_ptr<Mesh> rectMesh = ResMgr::Find<Mesh>(strKey::Default::mesh::RectMesh);
		rectMesh->BindBuffer();
		mergeMaterial->BindData();
		rectMesh->Render();


		RenderOpaque();
		RenderCutout();
		RenderTransparent();
		RenderPostProcess();
	}

	void Com_Camera::CreateViewMatrix()
	{
		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
		float3 pos = tr->GetRelativePos();

		// Crate Translate view matrix
		mView = MATRIX::Identity;
		mView *= MATRIX::CreateTranslation(-pos);
		//회전 정보

		float3 up = tr->Up();
		float3 right = tr->Right();
		float3 foward = tr->Forward();

		MATRIX viewRotate;
		viewRotate._11 = right.x; viewRotate._12 = up.x; viewRotate._13 = foward.x;
		viewRotate._21 = right.y; viewRotate._22 = up.y; viewRotate._23 = foward.y;
		viewRotate._31 = right.z; viewRotate._32 = up.z; viewRotate._33 = foward.z;

		mView *= viewRotate;
	}

	void Com_Camera::CreateProjectionMatrix()
	{
		uint2 resolution = GPUMgr::GetResolution();
		CreateProjectionMatrix(resolution.x, resolution.y);
	}

	void Com_Camera::CreateProjectionMatrix(uint ResolutionX, uint ResolutionY)
	{
		float width = (float)ResolutionX * mScale;
		float height = (float)ResolutionY * mScale;
		mAspectRatio = width / height;

		switch (mProjType)
		{
		case eProjectionType::Perspective:
			mProjection = MATRIX::CreatePerspectiveFieldOfViewLH
			(
				XM_2PI / 6.0f
				, mAspectRatio
				, mNear
				, mFar
			);
			break;
		case eProjectionType::Orthographic:
			mProjection = MATRIX::CreateOrthographicLH(width /*/ 100.0f*/, height /*/ 100.0f*/, mNear, mFar);
			break;
		default:
			MH_ASSERT(false);
			break;
		}
	}

	void Com_Camera::SetScale(float _scale)
	{
		if (_scale < 0.f)
			return;

		mScale = _scale;
		CreateProjectionMatrix();
	}

	void Com_Camera::RegisterCameraInRenderer()
	{
		//define::eSceneType type = SceneMgr::GetActiveScene()->GetSceneType();
		RenderMgr::RegisterCamera(this);
	}

	void Com_Camera::TurnLayerMask(define::eLayerType _layer, bool _enable)
	{
		mLayerMasks.set((uint)_layer, _enable);
	}


	//bool Com_Camera::FindRendererRecursive(GameObject* _pObj)
	//{
	//	bool bRet = false;
	//	if (_pObj)
	//	{
	//		if (_pObj->GetComponent(eComponentType::Renderer))
	//		{
	//			bRet = true;
	//		}
	//		else
	//		{
	//			//렌더러가 하나라도 있을 시 true 반환. 없을 시 false
	//			const auto& childs = _pObj->GetChilds();
	//			for (size_t i = 0; i < childs.size(); ++i)
	//			{
	//				bRet = FindRendererRecursive(childs[i]);
	//				if (bRet)
	//					break;
	//			}
	//		}
	//	}

	//	return bRet;
	//}

	void Com_Camera::SortGameObjects()
	{
		mDefferedOpaqueGameObjects.clear();
		mOpaqueGameObjects.clear();
		mCutoutGameObjects.clear();
		mTransparentGameObjects.clear();
		mPostProcessGameObjects.clear();

		IScene* scene = SceneMgr::GetActiveScene();
		for (int index = 0; index < (uint)define::eLayerType::END; index++)
		{
			if (mLayerMasks[index] == true)
			{
				Layer& layer = scene->GetLayer((define::eLayerType)index);
				GameObjects gameObjects = layer.GetGameObjects();
				if (gameObjects.size() == 0)
					continue;

				for (GameObject* obj : gameObjects)
				{
					PushGameObjectToRenderingModes(obj);
				}
			}
		}
	}

	void Com_Camera::RenderDeffered()
	{
		for (size_t i = 0; i < mDefferedOpaqueGameObjects.size(); ++i)
		{
			if (mDefferedOpaqueGameObjects[i])
			{
				mDefferedOpaqueGameObjects[i]->Render();
			}
		}
	}

	void Com_Camera::RenderOpaque()
	{
		for (GameObject* obj : mOpaqueGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Com_Camera::RenderCutout()
	{
		for (GameObject* obj : mCutoutGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Com_Camera::RenderTransparent()
	{
		for (GameObject* obj : mTransparentGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Com_Camera::RenderPostProcess()
	{

		for (GameObject* obj : mPostProcessGameObjects)
		{
			if (obj == nullptr)
				continue;

			RenderMgr::CopyRenderTarget();
			obj->Render();
		}
	}

	void Com_Camera::PushGameObjectToRenderingModes(GameObject* _gameObj)
	{
		if (nullptr == _gameObj)
			return;

		IRenderer* renderer = static_cast<IRenderer*>(_gameObj->GetComponent(define::eComponentType::Renderer));

		if (nullptr == renderer)
			return;

		eRenderingMode mode = eRenderingMode::None;
		Material* mtrl = renderer->GetCurrentMaterial(0u);
		if (mtrl)
		{
			mode = mtrl->GetRenderingMode();
		}

		switch (mode)
		{
		case eRenderingMode::DefferdOpaque:
			[[fallthrough]];
		case eRenderingMode::DefferdMask:
			mDefferedOpaqueGameObjects.push_back(_gameObj);
			break;
		case eRenderingMode::Opaque:
			mOpaqueGameObjects.push_back(_gameObj);
			break;
		case eRenderingMode::CutOut:
			mCutoutGameObjects.push_back(_gameObj);
			break;
		case eRenderingMode::Transparent:
			mTransparentGameObjects.push_back(_gameObj);
			break;
		case eRenderingMode::PostProcess:
			mPostProcessGameObjects.push_back(_gameObj);
			break;
		default:
			break;
		}
	}
}
