#include "EnginePCH.h"

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

	void IScript::Initialize()
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
