#pragma once
#include "IComponent.h"
#include "SimpleMath.h"



namespace mh
{
	

	class GameObject;
	class Com_Transform : public IComponent
	{
	public:
		Com_Transform();

		Com_Transform(const Com_Transform& _other) = default;
		CLONE(Com_Transform);

		virtual ~Com_Transform();

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		virtual void Init() final {};
		virtual void Update() final {};
		virtual void FixedUpdate();
		virtual void Render() {};

		void SetConstBuffer();
		

		float3 GetPosition() { return mPosition; };
		float3 GetRotation() { return mRotation; };
		float3 GetScale() { return mScale; };

		void SetPosition(const float3& _position) { mPosition = _position; };
		void SetRotation(const float3& _degree) { mRotation = _degree; };
		void SetScale(const float3& _scale) { mScale = _scale; };

		float3 Forward() { return mForward; }
		float3 Right() { return mRight; }
		float3 Up() { return mUp; }

		MATRIX& GetWorldMatrix() { return mWorld; }

	private:
		float3 mForward;
		float3 mRight;
		float3 mUp;
		float3 mPosition;
		float3 mRotation;
		float3 mScale;
		MATRIX mWorld;
	};
}
