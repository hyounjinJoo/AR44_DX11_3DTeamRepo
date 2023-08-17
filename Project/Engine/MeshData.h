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

		virtual eResult Save(const std::filesystem::path& _fileName) override;
		virtual eResult Load(const std::filesystem::path& _path) override;

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;
		GameObject* Instantiate();

	private:

		std::shared_ptr<Mesh>					mMesh;
		std::vector<std::shared_ptr<Material>>	mMaterials;
    };
}


