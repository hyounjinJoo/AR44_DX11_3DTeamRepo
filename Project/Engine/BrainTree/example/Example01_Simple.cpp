#include <iostream>
#include "../BrainTree.h"

namespace BrainTree
{
    class SayHello : public Node
    {
        Status update() override
        {
            std::cout << "Hello, World!" << std::endl;
            return Node::Status::Success;
        }
    };

    int Example()
    {
        BehaviorTree tree;

        auto repeater = std::make_shared<Repeater>(5);
        repeater->setChild(std::make_shared<SayHello>());
        tree.setRoot(repeater);

        // simulate 5 frames
        for (int i = 0; i < 5; i++)
            tree.update();

        return 0;
    }
}
