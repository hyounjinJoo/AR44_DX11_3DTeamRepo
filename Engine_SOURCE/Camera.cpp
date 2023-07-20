#include "EnginePCH.h"

#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
#include "Application.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Material.h"
#include "BaseRenderer.h"
#include "SceneManager.h"

extern mh::Application application;

namespace mh
{
	math::Matrix Camera::gView = math::Matrix::Identity;
	math::Matrix Camera::gProjection = math::Matrix::Identity;

	Camera::Camera()
		: IComponent(enums::eComponentType::Camera)
		, mType(eProjectionType::Orthographic)
		, mAspectRatio(1.0f)
		, mNear(1.0f)
		, mFar(1000.0f)
		, mScale(1.0f)
	{
		EnableLayerMasks();
	}

	Camera::~Camera()
	{
	}

	void Camera::Initialize()
	{

		RegisterCameraInRenderer();
	}

	void Camera::Update()
	{

	}

	void Camera::FixedUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix();

		RegisterCameraInRenderer();
	}

	void Camera::Render()
	{
		gView = mView;
		gProjection = mProjection;

		SortGameObjects();

		RenderOpaque();
		RenderCutout();
		RenderTransparent();
		RenderPostProcess();
	}

	void Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		math::Vector3 pos = tr->GetPosition();

		// Crate Translate view matrix
		mView = math::Matrix::Identity;
		mView *= math::Matrix::CreateTranslation(-pos);
		//회전 정보

		math::Vector3 up = tr->Up();
		math::Vector3 right = tr->Right();
		math::Vector3 foward = tr->Foward();

		math::Matrix viewRotate;
		viewRotate._11 = right.x; viewRotate._12 = up.x; viewRotate._13 = foward.x;
		viewRotate._21 = right.y; viewRotate._22 = up.y; viewRotate._23 = foward.y;
		viewRotate._31 = right.z; viewRotate._32 = up.z; viewRotate._33 = foward.z;

		mView *= viewRotate;
	}

	void Camera::CreateProjectionMatrix()
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

	void Camera::RegisterCameraInRenderer()
	{
		enums::eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		renderer::gCameras[(UINT)type].push_back(this);
	}

	void Camera::TurnLayerMask(enums::eLayerType _layer, bool _enable)
	{
		mLayerMasks.set((UINT)_layer, _enable);
	}

	void Camera::SortGameObjects()
	{
		mOpaqueGameObjects.clear();
		mCutoutGameObjects.clear();
		mTransparentGameObjects.clear();
		mPostProcessGameObjects.clear();

		Scene* scene = SceneManager::GetActiveScene();
		for (int index = 0; index < (UINT)enums::eLayerType::End; index++)
		{
			if (mLayerMasks[index] == true)
			{
				Layer& layer = scene->GetLayer((enums::eLayerType)index);
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

	void Camera::RenderOpaque()
	{
		for (GameObject* obj : mOpaqueGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::RenderCutout()
	{
		for (GameObject* obj : mCutoutGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::RenderTransparent()
	{
		for (GameObject* obj : mTransparentGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::RenderPostProcess()
	{

		for (GameObject* obj : mPostProcessGameObjects)
		{
			if (obj == nullptr)
				continue;
			renderer::CopyRenderTarget();
			obj->Render();
		}
	}

	void Camera::PushGameObjectToRenderingModes(GameObject* _gameObj)
	{
		BaseRenderer* renderer
			= _gameObj->GetComponent<BaseRenderer>();
		if (renderer == nullptr)
			return;

		std::shared_ptr<graphics::Material> material = renderer->GetMaterial();
		//if (material == nullptr)
		//	continue;

		graphics::eRenderingMode mode = material->GetRenderingMode();

		switch (mode)
		{
		case graphics::eRenderingMode::Opaque:
			mOpaqueGameObjects.push_back(_gameObj);
			break;
		case graphics::eRenderingMode::CutOut:
			mCutoutGameObjects.push_back(_gameObj);
			break;
		case graphics::eRenderingMode::Transparent:
			mTransparentGameObjects.push_back(_gameObj);
			break;
		case graphics::eRenderingMode::PostProcess:
			mPostProcessGameObjects.push_back(_gameObj);
			break;
		default:
			break;
		}
	}
}
