#pragma once
#include "Singleton.h"

#include <unordered_map>
#include <typeindex>

namespace mh
{
    class IComponent;
    class ComMgr
        : public Singleton<ComMgr>
    {
        SINGLETON(ComMgr);

    private:
        std::unordered_map<std::string_view, std::function<IComponent* ()>> mUmapComConstructor;
        std::unordered_map<std::type_index, const std::string_view>         mUmapComName;
        
    public:
        template <typename T>
        void AddComConstructor(const std::string_view _strKey);

        
        IComponent* GetNewCom(const std::string_view _strKey);
        const std::string_view GetComName(const std::type_info& _typeid_T_);
        template <typename T>
        const std::string_view GetComName();
    };

    template <typename T>
    inline void ComMgr::AddComConstructor(const std::string_view _strKey)
    {
        static_assert(std::is_base_of_v<IComponent, T>);
        mUmapComName.insert(std::make_pair(std::type_index(typeid(T)), _strKey));
        mUmapComConstructor.insert(std::make_pair(_strKey,
            []()->T*
            {
                return new T;
            }
        ));
    }

    template<typename T>
    inline const std::string_view ComMgr::GetComName()
    {
        return GetComName(typeid(T));
    }
}



