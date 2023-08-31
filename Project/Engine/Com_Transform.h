#pragma once
#include "ITransform.h"
#include "DefaultShader/SH_CommonStruct.hlsli"
#include "TimeMgr.h"



namespace mh
{
	class Com_Camera;
    class GameObject;
    class Com_Transform :
        public ITransform
    {
    public:
        Com_Transform();

        //단순 Value만 저장 중이므로 기본 복사 생성자로도 충분함.
        Com_Transform(const Com_Transform& _other) = default;
        CLONE(Com_Transform);

        virtual ~Com_Transform() override;

	public:

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;

		/* 월드 행렬을 업데이트한다 */
		virtual void  FixedUpdate() override;

		virtual void BindData() override;

		void  SetScale(const float3& _scale) { mScale = _scale; }
		void  SetRotation(const float3& _rotation);
		void  SetRotation(define::eAxis3D _eAxis, float _degree);
		void  SetPosition(const float3& _position);
		void  SetPosition(define::eAxis3D _eAxis, float _position);

		void SetScaleXY(const float2& _scale) { float3(mScale.x = _scale.x, mScale.y = _scale.y, 1.0f); }

		void SetScaleFromTool(const float3& _scale);
		void SetRotationFromTool(const float3& _rotation);
		void SetPositionFromTool(const float3& _position);

		/* 콜라이더를 제외하고 오브젝트만 회전시키는 함수 */
		void  SetRotationExcludingColliders(const float3& _rotation);
		void  SetRotationExcludingColliders(define::eAxis3D _eAxis, float _degree);

		/* 콜라이더를 제외하고 오브젝트만 이동시키는 함수 */
		void  SetPositionExcludingColliders(const float3& _position);
		void  SetPositionExcludingColliders(define::eAxis3D _eAxis, float _position);

		void SetWorldMatrix(const math::Matrix& _matrix);
		void SetUpdateByMat(bool _bFlag) { mbUpdateByMat = _bFlag; }
		bool GetUpdateByMat() { return mbUpdateByMat; }

		void DecomposeWorld();
		float NormalizeAngle(float _angle);
		float3 NormalizeAngles(float3 _angles);
		float3 QuaternionToEuler(float _x, float _y, float _z, float _w);
		physx::PxQuat EulerToQuaternion(float3 _euler);

		float GetBoundingSphereRadius() { return max(max(mScale.x, mScale.y), mScale.z); }

		float3 GetWorldPosition();
		const float3& GetPosition();
		const float3& GetRotation();
		const float3& GetScale();

		float3 GetRight() { return mMatWorld.Right(); }
		float3 GetUp() { return mMatWorld.Up(); }
		float3 GetForward() { return mMatWorld.Forward(); } 
		float3 GetLook() { return mMatWorld.Backward(); }

		void AddRotation(define::eAxis3D _eAxis, float _degree);

		void Move(const float3& _velocity);

		const math::Matrix& GetWorldMatrix() { return mMatWorld; }
		const math::Matrix& GetWorldMatrix() const { return mMatWorld; }


	

	private:
		Com_Transform* mpParent;

		float3 mScale;
		float3 mRotation;
		float3 mRelativeRotation;
		float3 mPosition;
		float3 mRelativePosition;


		math::Matrix  mMatLocal;
		math::Matrix  mMatWorld;

		bool mbUpdateByMat;

		tCB_Transform mCB_Transform;
	};
}
