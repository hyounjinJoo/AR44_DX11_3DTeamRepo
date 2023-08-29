#pragma once

#include <unordered_map>
#include <typeindex>
#include <string>
#include <functional>

namespace mh
{
    class IComponent;
    class ComMgr
    {
        friend class Application;
    public:
        template <typename T>
        static void AddComConstructor(const std::string_view _strKey);

        static IComponent* GetNewCom(const std::string_view _strKey);

        static const std::string_view GetComName(const std::type_info& _typeid_T_);

        template <typename T>
        static const std::string_view GetComName();

        template <typename T>
        static inline UINT32 GetComTypeID(const T* _typePtr = nullptr);
        
    private:
        static void Init();
        static void Release();

        static inline UINT32 GetNextComTypeID();

    private:
        static std::unordered_map<std::string_view, std::function<IComponent* ()>> mUmapComConstructor;
        static std::unordered_map<std::type_index, const std::string_view>         mUmapComName;
    };

    template <typename T>
    static inline void ComMgr::AddComConstructor(const std::string_view _strKey)
    {
        static_assert(std::is_base_of_v<IComponent, T>);
        mUmapComName.insert(std::make_pair(std::type_index(typeid(T)), _strKey));
        mUmapComConstructor.insert(std::make_pair(_strKey,
            []()->T*
            {
                T* com = new T;
                com->SetComTypeID(GetComTypeID<T>());
                return com;
            }
        ));
    }

    template<typename T>
    static inline const std::string_view ComMgr::GetComName()
    {
        return GetComName(typeid(T));
    }

    inline UINT32 ComMgr::GetNextComTypeID()
    {
        static UINT32 lastID = 0u;
        return ++lastID;
    }

    template<typename T>
    inline UINT32 ComMgr::GetComTypeID(const T* _typePtr)
    {
        static_assert(std::is_base_of_v<IComponent, T>);
        static UINT32 typeId = GetNextComTypeID();
        return typeId;
    }

}



