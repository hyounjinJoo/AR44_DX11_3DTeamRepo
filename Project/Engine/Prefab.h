#pragma once

#include "IRes.h"

namespace mh
{
    class GameObject;
	class Prefab
		: public IRes
	{
    public:
        Prefab();
        virtual ~Prefab();
        
        virtual eResult Save(const std::fs::path& _filePath) override;                                                                                   
        virtual eResult Load(const std::fs::path& _filePath) override;

        //virtual eResult SaveJson(Json::Value* _pJVal) override;
        //virtual eResult LoadJson(const Json::Value* _pJVal) override;

        //DontDelete를 true로 설정할 시에는 prefab이 제거될 때 관리하는 게임오브젝트를 삭제하지 않음.
        inline void     RegisterPrefab(GameObject* _pPrefab, bool _bDontDelete = false);
        bool            IsAvailable() const { return nullptr != mPrefab; }
        GameObject*     Instantiate();


    private:
        GameObject* mPrefab;
        bool         mbDontDelete;
	};


    inline void Prefab::RegisterPrefab(GameObject* _pPrefab, bool _bDontDelete)
    {
        mPrefab = _pPrefab;
        mbDontDelete = _bDontDelete;
        
    }
}


