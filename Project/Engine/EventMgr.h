#pragma once
#include "define_Enum.h"

namespace mh
{
    class Prefab;
    class GameObject;
    class EventMgr
    {
        friend class Application;
    public:
        enum class eEventType
        {
            SpawnGameObj,
            DeleteGameObj,
            AddChild,
        };
        struct tEvent
        {
            eEventType Type;
            DWORD_PTR	wParam;
            DWORD_PTR	lParam;
        };
    public:
        static GameObject* SpawnGameObject(GameObject* _gameObj, define::eLayerType _layer);
        //static GameObject* SpawnGameObject(GameObject* _pNewObject, const float3& _vWorldPos);
        //static GameObject* SpawnGameObject(GameObject* _pNewObject, int _LayerIdx = -1);
        static void DestroyGameObj(GameObject* _pObject);
        static void AddChildGameObj(GameObject* _pParent, GameObject* _pChild);

        //프리팹(리소스)를 넣어서 게임오브젝트를 스폰시킬 경우 해당 게임오브젝트의 주소를 리턴해줌(추가적인 설정을 할수있게)	
        //static GameObject* SpawnPrefab2D(std::shared_ptr<Prefab> _Prefab, const float2& _vWorldPosXY);
        //static GameObject* SpawnPrefab(std::shared_ptr<Prefab> _Prefab, const float3& _vWorldPos);
        //static GameObject* SpawnPrefab(std::shared_ptr<Prefab> _Prefab);


    private:
        static bool Init();
        static void Release();

        static void Update();

        static void AddEvent(const tEvent& _event) { mEvents.push_back(_event); }
        static bool IsLevelChanged() { return mbLevelModified; }

        //lParam = GameObject Pointer
        //wParam = Layer Number
        static void SpawnNewGameObj(const tEvent& _event);

        //lParam = GameObject Pointer
        //wParam = None
        static void DestroyGameObj(const tEvent& _event);

        //lParam = Parent GameObject*
        //wParam = Child GameObject*
        //SpawnNewGameObj는 따로 해주지 않으므로 주의
        static void AddChildGameObj(const tEvent& _event);



    private:
        static void ProcessEvent();
        static void ProcessLazyEvent();

    private:
        static std::vector<tEvent> mEvents;

        //한 프레임 기다렸다 처리해야 하는 이벤트는 여기에 저장
        static std::vector<tEvent> mLazyEvents;

        static bool           mbLevelModified;


    private:
        EventMgr() = delete;
        ~EventMgr() = delete;
    };
}


