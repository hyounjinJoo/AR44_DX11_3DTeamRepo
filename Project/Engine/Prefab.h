#pragma once

#include "IRes.h"

namespace mh
{
	class Prefab
		: public IRes
	{
	public:
		Prefab();
		virtual ~Prefab();

	public:
		virtual eResult Save(const std::filesystem::path& _FilePath) override;
		virtual eResult Load(const std::filesystem::path& _FilePath) override;

	};
}


