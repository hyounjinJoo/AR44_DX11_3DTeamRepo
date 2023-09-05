#pragma once
#include <Engine/IScript.h>

namespace mh
{
    class Script_Player :
        public IScript
    {
    public:
        Script_Player() {};
        virtual ~Script_Player() {};

        virtual void Update() override;
    };

}

