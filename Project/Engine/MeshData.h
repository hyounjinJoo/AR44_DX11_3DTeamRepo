#pragma once
#include "IRes.h"
#include "define_Struct.h"
#include "define_GPU.h"

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
		virtual eResult Load(const std::filesystem::path& _fileName) override;

		virtual eResult SaveJson(Json::Value* _pJson) override;
		virtual eResult LoadJson(const Json::Value* _pJson) override;
		GameObject* Instantiate();

	private:
		eResult LoadFromFBX(const std::filesystem::path& _fileName);

	private:
		std::vector<tMeshContainer> mMeshContainers;
    };
}


