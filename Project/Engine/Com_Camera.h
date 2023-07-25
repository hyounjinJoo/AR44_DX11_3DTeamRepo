#pragma once
#include "IComponent.h"

namespace  mh
{
	class Com_Camera : public IComponent
	{
	public:
		enum eProjectionType
		{
			Perspective,
			Orthographic,
		};

		__forceinline static math::Matrix& GetGpuViewMatrix() { return gView; }
		__forceinline static math::Matrix& GetGpuProjectionMatrix() { return gProjection; }
		__forceinline static void SetGpuViewMatrix(math::Matrix _view) { gView = _view; }
		__forceinline static void SetGpuProjectionMatrix(math::Matrix _projection) { gProjection = _projection; }

		Com_Camera();
		virtual ~Com_Camera();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void CreateViewMatrix();
		void CreateProjectionMatrix();
		void RegisterCameraInRenderer();

		void TurnLayerMask(define::eLayerType _layer, bool _enable = true);
		void EnableLayerMasks() { mLayerMasks.set(); }
		void DisableLayerMasks() { mLayerMasks.reset(); }

		void SetProjectionType(eProjectionType _type) { mType = _type; }

		float GetScale() const { return mScale; }
		const math::Matrix& GetViewMatrix() const { return mView; }
		const math::Matrix& GetProjectionMatrix() const { return mProjection; }

	private:
		void SortGameObjects();
		void RenderOpaque();
		void RenderCutout();
		void RenderTransparent();
		void RenderPostProcess();
		void PushGameObjectToRenderingModes(GameObject* _gameObj);

	private:
		static math::Matrix gView;
		static math::Matrix gProjection;

		math::Matrix mView;
		math::Matrix mProjection;

		eProjectionType mType;
		float mAspectRatio;

		float mNear;
		float mFar;
		float mScale;

		std::bitset<(UINT)define::eLayerType::End> mLayerMasks;
		std::vector<GameObject*> mOpaqueGameObjects;
		std::vector<GameObject*> mCutoutGameObjects;
		std::vector<GameObject*> mTransparentGameObjects;
		std::vector<GameObject*> mPostProcessGameObjects;
	};
}
