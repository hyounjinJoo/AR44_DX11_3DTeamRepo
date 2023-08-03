#include "EnginePCH.h"
#include "ICollider3D.h"
#include "Com_Transform.h"

namespace mh
{
	ICollider3D::ICollider3D()
		: ICollider(define::eColliderType::None)
	{
		MH_ASSERT(TRUE);
	}
	
	ICollider3D::ICollider3D(const ICollider3D& _collider)
		: ICollider(_collider.mType)
	{
		MH_ASSERT(TRUE);
	}

	ICollider3D::~ICollider3D()
	{
	}

	void ICollider3D::Init()
	{
	}

	void ICollider3D::Update()
	{
	}

	void ICollider3D::FixedUpdate()
	{
	}

	void ICollider3D::Render()
	{
	}
}
