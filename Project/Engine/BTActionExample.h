#pragma once
#include "BrainTree\BrainTree.h"

namespace mh
{
	class BTActionExample : public BrainTree::Leaf
	{
	public:
		// construtor destructor
		BTActionExample();
		BTActionExample(std::shared_ptr<BrainTree::Blackboard> blackboard);
		virtual ~BTActionExample();

		// delete Function
		BTActionExample(const BTActionExample& _Other) = delete;
		BTActionExample(BTActionExample&& _Other) noexcept = delete;
		BTActionExample& operator=(const BTActionExample& _Other) = delete;
		BTActionExample& operator=(BTActionExample&& _Other) noexcept = delete;

	public:
		// main Logic. must have override update() function. It is pure virtual function.
		virtual BrainTree::Node::Status update() override;
	};
}
