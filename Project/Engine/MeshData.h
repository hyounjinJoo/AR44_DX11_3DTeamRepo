#pragma once
#include "IRes.h"

namespace mh
{
	class Mesh;
	class Material;
	class GameObject;
    class MeshData :
        public IRes
    {
	public:
		MeshData();
		virtual ~MeshData();

		std::shared_ptr<Mesh>					mMesh;
		std::vector<std::shared_ptr<Material>>	mMaterials;

	public:
		virtual eResult Load(const std::filesystem::path& _path) override;

		GameObject* Instantiate();

	public:

    };
}


