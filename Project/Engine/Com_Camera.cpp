
#include "EnginePCH.h"

#include "Com_Camera.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "Application.h"
#include "RenderMgr.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Material.h"
#include "IRenderer.h"
#include "SceneManager.h"

extern mh::Application application;

namespace mh
{
	math::Matrix Com_Camera::gView = math::Matrix::Identity;
	math::Matrix Com_Camera::gProjection = math::Matrix::Identity;

	Com_Camera::Com_Camera()
		: IComponent(define::eComponentType::Camera)
		, mType(eProjectionType::Orthographic)
		, mAspectRatio(1.0f)
		, mNear(1.0f)
		, mFar(1000.0f)
		, mScale(1.0f)
	{
		EnableLayerMasks();
	}

	Com_Camera::~Com_Camera()
	{
	}

	void Com_Camera::Initialize()
	{

		RegisterCameraInRenderer();
	}

	void Com_Camera::Update()
	{

	}

	void Com_Camera::FixedUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix();

		RegisterCameraInRenderer();
	}

	void Com_Camera::Render()
	{
		gView = mView;
		gProjection = mProjection;

		SortGameObjects();

		RenderOpaque();
		RenderCutout();
		RenderTransparent();
		RenderPostProcess();
	}

	void Com_Camera::CreateViewMatrix()
	{
		Com_Transform& tr = GetOwner()->GetTransform();
		math::Vector3 pos = tr.GetPosition();

		// Crate Translate view matrix
		mView = math::Matrix::Identity;
		mView *= math::Matrix::CreateTranslation(-pos);
		//회전 정보

		math::Vector3 up = tr.Up();
		math::Vector3 right = tr.Right();
		math::Vector3 foward = tr.Foward();

		math::Matrix viewRotate;
		viewRotate._11 = right.x; viewRotate._12 = up.x; viewRotate._13 = foward.x;
		viewRotate._21 = right.y; viewRotate._22 = up.y; viewRotate._23 = foward.y;
		viewRotate._31 = right.z; viewRotate._32 = up.z; viewRotate._33 = foward.z;

		mView *= viewRotate;
	}

	void Com_Camera::CreateProjectionMatrix()
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = (winRect.right - winRect.left) * mScale;
		float height = (winRect.bottom - winRect.top) * mScale;
		mAspectRatio = width / height;

		if (mType == eProjectionType::Perspective)
		{
			mProjection = math::Matrix::CreatePerspectiveFieldOfViewLH
			(
				XM_2PI / 6.0f
				, mAspectRatio
				, mNear
				, mFar
			);
		}
		else
		{
			mProjection = math::Matrix::CreateOrthographicLH(width /*/ 100.0f*/, height /*/ 100.0f*/, mNear, mFar);
		}
	}

	void Com_Camera::RegisterCameraInRenderer()
	{
		define::eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		RenderMgr::GetInst()->RegisterCamera(type, this);
	}

	void Com_Camera::TurnLayerMask(define::eLayerType _layer, bool _enable)
	{
		mLayerMasks.set((UINT)_layer, _enable);
	}

	void Com_Camera::SortGameObjects()
	{
		mOpaqueGameObjects.clear();
		mCutoutGameObjects.clear();
		mTransparentGameObjects.clear();
		mPostProcessGameObjects.clear();

		Scene* scene = SceneManager::GetActiveScene();
		for (int index = 0; index < (UINT)define::eLayerType::End; index++)
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

			RenderMgr::GetInst()->CopyRenderTarget();
			obj->Render();
		}
	}

	void Com_Camera::PushGameObjectToRenderingModes(GameObject* _gameObj)
	{
		IRenderer* renderer
			= _gameObj->GetComponent<IRenderer>();
		if (renderer == nullptr)
			return;

		std::shared_ptr<GPU::Material> material = renderer->GetMaterial();
		//if (material == nullptr)
		//	continue;

		GPU::eRenderingMode mode = material->GetRenderingMode();

		switch (mode)
		{
		case GPU::eRenderingMode::Opaque:
			mOpaqueGameObjects.push_back(_gameObj);
			break;
		case GPU::eRenderingMode::CutOut:
			mCutoutGameObjects.push_back(_gameObj);
			break;
		case GPU::eRenderingMode::Transparent:
			mTransparentGameObjects.push_back(_gameObj);
			break;
		case GPU::eRenderingMode::PostProcess:
			mPostProcessGameObjects.push_back(_gameObj);
			break;
		default:
			break;
		}
	}
}
