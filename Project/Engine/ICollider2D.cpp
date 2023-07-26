#include "EnginePCH.h"
#include "ICollider2D.h"

#include "GameObject.h"

#include "RenderMgr.h"
#include "IScript.h"

namespace mh
{
	UINT ICollider2D::gColliderNumber = 0;
	ICollider2D::ICollider2D()
		: IComponent(define::eComponentType::Collider)
		, mType(define::eColliderType::None)
		, mTransform(nullptr)
		, mSize(math::Vector2::One)
		, mCenter(math::Vector2::Zero)
		, mbTrigger(false)
		, mID(0)
		, mRadius(0.0f)
	{
		mID = gColliderNumber++;
	}

	ICollider2D::~ICollider2D()
	{
	}

	void ICollider2D::Initialize()
	{
		mTransform = &(GetOwner()->GetTransform());
	}

	void ICollider2D::Update()
	{
	}

	void ICollider2D::FixedUpdate()
	{
		math::Vector3 scale = mTransform->GetScale();
		scale *= math::Vector3(mSize.x, mSize.y, 1.0f);

		math::Vector3 rotation = mTransform->GetRotation();

		math::Vector3 position = mTransform->GetPosition();
		math::Vector3 colliderPos = position + math::Vector3(mCenter.x, mCenter.y, 0.0f);
		mPosition = colliderPos;

		math::Matrix scaleMatrix = math::Matrix::CreateScale(scale);
		math::Matrix rotationMatrix;
		rotationMatrix = math::Matrix::CreateRotationX(rotation.x);
		rotationMatrix *= math::Matrix::CreateRotationY(rotation.y);
		rotationMatrix *= math::Matrix::CreateRotationZ(rotation.z);

		math::Matrix positionMatrix;
		positionMatrix.Translation(math::Vector3(colliderPos.x, colliderPos.y, colliderPos.z));

		math::Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		GPU::tDebugMesh meshAttribute = {};
		meshAttribute.position = math::Vector3(colliderPos.x, colliderPos.y, colliderPos.z);
		meshAttribute.radius = mRadius;
		meshAttribute.rotatation = rotation;
		meshAttribute.scale = scale;
		meshAttribute.type = mType;

		renderer::gDebugMeshes.push_back(meshAttribute);
	}

	void ICollider2D::Render()
	{
	}

	void ICollider2D::OnCollisionEnter(ICollider2D* _collider)
	{
		const std::vector<IScript*>& scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnCollisionEnter(_collider);
		}
	}

	void ICollider2D::OnCollisionStay(ICollider2D* _collider)
	{
		const std::vector<IScript*>& scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnCollisionStay(_collider);
		}
	}

	void ICollider2D::OnCollisionExit(ICollider2D* _collider)
	{
		const std::vector<IScript*>& scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnCollisionExit(_collider);
		}
	}

	void ICollider2D::OnTriggerEnter(ICollider2D* _collider)
	{
		const std::vector<IScript*>& scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnTriggerEnter(_collider);
		}
	}

	void ICollider2D::OnTriggerStay(ICollider2D* _collider)
	{
		const std::vector<IScript*>& scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnTriggerStay(_collider);
		}
	}

	void ICollider2D::OnTriggerExit(ICollider2D* _collider)
	{
		const std::vector<IScript*>& scripts = GetOwner()->GetScripts();
		for (IScript* script : scripts)
		{
			script->OnTriggerExit(_collider);
		}
	}

}
