#include "EnginePCH.h"

#include "Prefab.h"

namespace mh
{
	Prefab::Prefab()
		: IRes(eResourceType::Prefab)
	{
	}

	Prefab::~Prefab()
	{
	}
	eResult Prefab::Save(const std::filesystem::path& _FilePath)
	{
		return eResult::Fail_NotImplemented;
	}
	eResult Prefab::Load(const std::filesystem::path& _FilePath)
	{
		return eResult::Fail_NotImplemented;
	}
}

