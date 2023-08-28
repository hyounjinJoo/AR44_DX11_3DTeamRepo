#pragma once
#include "IComponent.h"

#include "define_Enum.h"

namespace  mh
{
	class Com_Camera : public IComponent
	{
	public:
		Com_Camera();
		virtual ~Com_Camera();

		__forceinline static const MATRIX& GetGpuViewMatrix() { return gView; }
		__forceinline static const MATRIX& GetGpuViewInvMatrix() { return gViewInverse; }
		__forceinline static const MATRIX& GetGpuProjectionMatrix() { return gProjection; }
		__forceinline static void SetGpuViewMatrix(MATRIX _view) { gView = _view; }
		__forceinline static void SetGpuProjectionMatrix(MATRIX _projection) { gProjection = _projection; }

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;

		//이 함수는 RenderMgr가 호출
		void RenderCamera();

		void CreateViewMatrix();
		
		void RegisterCameraInRenderer();

		void TurnLayerMask(define::eLayerType _layer, bool _enable = true);
		void EnableLayerMasks() { mLayerMasks.set(); }
		void DisableLayerMasks() { mLayerMasks.reset(); }

		void SetProjectionType(define::eProjectionType _type) { mProjType = _type; CreateProjectionMatrix(); }
		define::eProjectionType GetProjectionType() const { return mProjType; }

		void CreateProjectionMatrix();
		void CreateProjectionMatrix(uint ResolutionX, uint ResolutionY);
		
		void SetScale(float _scale);
		float GetScale() const { return mScale; }
		const MATRIX& GetViewMatrix() const { return mView; }
		const MATRIX& GetProjectionMatrix() const { return mProjection; }

	private:
		void SortGameObjects();
		void RenderDeffered();
		void RenderOpaque();
		void RenderCutout();
		void RenderTransparent();
		void RenderPostProcess();
		void PushGameObjectToRenderingModes(GameObject* _gameObj);

	private:
		static MATRIX gView;
		static MATRIX gViewInverse;
		static MATRIX gProjection;

		MATRIX mView;
		MATRIX mViewInverse;
		MATRIX mProjection;

		define::eProjectionType mProjType;
		float mAspectRatio;

		float mNear;
		float mFar;
		float mScale;

		std::bitset<(uint)define::eLayerType::END> mLayerMasks;
		std::vector<GameObject*> mDefferedOpaqueGameObjects;
		std::vector<GameObject*> mOpaqueGameObjects;
		std::vector<GameObject*> mCutoutGameObjects;
		std::vector<GameObject*> mTransparentGameObjects;
		std::vector<GameObject*> mPostProcessGameObjects;
	};



}
