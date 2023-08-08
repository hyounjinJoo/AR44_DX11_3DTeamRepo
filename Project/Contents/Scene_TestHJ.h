#pragma once

#include <Engine/IScene.h>

namespace mh
{
    class Scene_TestHJ :
        public IScene
    {
    public:
        Scene_TestHJ();
        virtual ~Scene_TestHJ();

        virtual void OnEnter() override {}

    private:
        int i;
    };
}


