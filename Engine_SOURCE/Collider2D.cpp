#include "Collider2D.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Script.h"

namespace mh
{
	UINT Collider2D::gColliderNumber = 0;
	Collider2D::Collider2D()
		: Component(enums::eComponentType::Collider)
		, mType(enums::eColliderType::None)
		, mTransform(nullptr)
		, mSize(math::Vector2::One)
		, mCenter(math::Vector2::Zero)
		, mbTrigger(false)
		, mID(0)
		, mRadius(0.0f)
	{
		mID = gColliderNumber++;
	}

	Collider2D::~Collider2D()
	{
	}

	void Collider2D::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}

	void Collider2D::Update()
	{
	}

	void Collider2D::FixedUpdate()
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

		graphics::DebugMesh meshAttribute = {};
		meshAttribute.position = math::Vector3(colliderPos.x, colliderPos.y, colliderPos.z);
		meshAttribute.radius = mRadius;
		meshAttribute.rotatation = rotation;
		meshAttribute.scale = scale;
		meshAttribute.type = mType;

		renderer::debugMeshes.push_back(meshAttribute);
	}

	void Collider2D::Render()
	{
	}

	void Collider2D::OnCollisionEnter(Collider2D* _collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionEnter(_collider);
		}
	}

	void Collider2D::OnCollisionStay(Collider2D* _collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionStay(_collider);
		}
	}

	void Collider2D::OnCollisionExit(Collider2D* _collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionExit(_collider);
		}
	}

	void Collider2D::OnTriggerEnter(Collider2D* _collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerEnter(_collider);
		}
	}

	void Collider2D::OnTriggerStay(Collider2D* _collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerStay(_collider);
		}
	}

	void Collider2D::OnTriggerExit(Collider2D* _collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerExit(_collider);
		}
	}

}
