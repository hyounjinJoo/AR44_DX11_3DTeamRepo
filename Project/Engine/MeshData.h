#pragma once
#include "IRes.h"
#include "json-cpp/json-forwards.h"

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
		virtual eResult Save(const std::filesystem::path& _filePath) override;
		//virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult Load(const std::filesystem::path& _path) override;
		GameObject* Instantiate();
    };
}


