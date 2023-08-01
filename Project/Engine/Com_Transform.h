#pragma once
#include "IComponent.h"
#include "SimpleMath.h"



namespace mh
{
	using namespace mh::math;

	class GameObject;
	class Com_Transform : public IComponent
	{
	public:
		Com_Transform();

		//"깊은 복사"가 필요하지 않은 일반 Value들만 가진 컴포넌트의 경우 = default;를 넣어주면 기본 복사 생성자가 호출됨.
		//값들을 그대로 복사
		//만약 고유하게 가지고 있는 동적할당된 포인터 주소를 가지고 있을 경우에는 직접 정의해줘야 함
		Com_Transform(const Com_Transform& _other) = default;
		CLONE(Com_Transform);

		virtual ~Com_Transform();

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		
		//LoadJson의 경우에는 const
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		virtual void Init() final {};
		virtual void Update() final {};
		virtual void FixedUpdate();
		virtual void Render() {};

		void SetConstBuffer();
		

		Vector3 GetPosition() { return mPosition; };
		Vector3 GetRotation() { return mRotation; };
		Vector3 GetScale() { return mScale; };

		void SetPosition(const Vector3& _position) { mPosition = _position; };
		void SetRotation(const Vector3& _degree) { mRotation = _degree; };
		void SetScale(const Vector3& _scale) { mScale = _scale; };

		Vector3 Foward() { return mForward; }
		Vector3 Right() { return mRight; }
		Vector3 Up() { return mUp; }

		Matrix& GetWorldMatrix() { return mWorld; }

	private:
		Vector3 mForward;
		Vector3 mRight;
		Vector3 mUp;
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;
		Matrix mWorld;
	};
}
