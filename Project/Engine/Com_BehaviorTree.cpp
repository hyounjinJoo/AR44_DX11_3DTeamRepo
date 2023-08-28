#include "PCH_Engine.h"

#include "Com_BehaviorTree.h"

namespace mh
{
	Com_BehaviorTree::Com_BehaviorTree()
		: IComponent(define::eComponentType::BehaviorTree)
		, mTree(new BrainTree::BehaviorTree())
		, mBlackBoard(mTree->getBlackboard())
	{

	}

	Com_BehaviorTree::~Com_BehaviorTree()
	{
		mBlackBoard = nullptr;
		mTree = nullptr;
	}

	void Com_BehaviorTree::Init()
	{
	}

	void Com_BehaviorTree::Update()
	{
		if (mTree)
		{
			mTree->update();
		}
	}

	void Com_BehaviorTree::FixedUpdate()
	{

	}

}
