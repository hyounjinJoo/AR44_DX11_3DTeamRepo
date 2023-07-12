#include "yaCollider2D.h"
#include "yaGameObject.h"
#include "yaRenderer.h"
#include "yaScript.h"

namespace ya
{
	UINT Collider2D::ColliderNumber = 0;
	Collider2D::Collider2D()
		: Component(eComponentType::Collider)
		, mType(eColliderType::None)
		, mTransform(nullptr)
		, mSize(Vector2::One)
		, mCenter(Vector2::Zero)
		, mbTrigger(false)
		, mID(0)
		, mRadius(0.0f)
	{
		mID = ColliderNumber++;
	}

	Collider2D::~Collider2D()
	{
	}

	void Collider2D::Initalize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}

	void Collider2D::Update()
	{
	}

	void Collider2D::FixedUpdate()
	{
		Vector3 scale = mTransform->GetScale();
		scale *= Vector3(mSize.x, mSize.y, 1.0f);

		Vector3 rotation = mTransform->GetRotation();

		Vector3 position = mTransform->GetPosition();
		Vector3 colliderPos = position + Vector3(mCenter.x, mCenter.y, 0.0f);
		mPosition = colliderPos;

		Matrix scaleMatrix = Matrix::CreateScale(scale);
		Matrix rotationMatrix;
		rotationMatrix = Matrix::CreateRotationX(rotation.x);
		rotationMatrix *= Matrix::CreateRotationY(rotation.y);
		rotationMatrix *= Matrix::CreateRotationZ(rotation.z);

		Matrix positionMatrix;
		positionMatrix.Translation(Vector3(colliderPos.x, colliderPos.y, colliderPos.z));

		Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		DebugMesh meshAttribute = {};
		meshAttribute.position = Vector3(colliderPos.x, colliderPos.y, colliderPos.z);
		meshAttribute.radius = mRadius;
		meshAttribute.rotatation = rotation;
		meshAttribute.scale = scale;
		meshAttribute.type = mType;

		renderer::debugMeshes.push_back(meshAttribute);
	}

	void Collider2D::Render()
	{
	}

	void Collider2D::OnCollisionEnter(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionEnter(collider);
		}
	}

	void Collider2D::OnCollisionStay(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionStay(collider);
		}
	}

	void Collider2D::OnCollisionExit(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionExit(collider);
		}
	}

	void Collider2D::OnTriggerEnter(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerEnter(collider);
		}
	}

	void Collider2D::OnTriggerStay(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerStay(collider);
		}
	}

	void Collider2D::OnTriggerExit(Collider2D* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerExit(collider);
		}
	}

}
