#include "PCH_Engine.h"

#include "IScript.h"



namespace mh
{
	IScript::IScript()
		: IComponent(define::eComponentType::Scripts)
	{

	}

	IScript::~IScript()
	{
	}

	void IScript::Init()
	{
	}

	void IScript::Update()
	{
	}

	void IScript::FixedUpdate()
	{
	}
	void IScript::Render()
	{
	}
}
