#pragma once
#include "IComponent.h"
#include "BrainTree\BrainTree.h"

namespace mh
{
	class Com_BehaviorTree : public IComponent
	{
	public:
		Com_BehaviorTree();
		CLONE_DISABLE(Com_BehaviorTree);

		virtual ~Com_BehaviorTree();

		//virtual eResult SaveJson(Json::Value* _pJVal) override;
		//virtual eResult LoadJson(const Json::Value* _pJVal) override;

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;

		// delete Function
		Com_BehaviorTree(const Com_BehaviorTree& _Other) = delete;
		Com_BehaviorTree(Com_BehaviorTree&& _Other) noexcept = delete;
		Com_BehaviorTree& operator=(const Com_BehaviorTree& _Other) = delete;
		Com_BehaviorTree& operator=(Com_BehaviorTree&& _Other) noexcept = delete;

		std::shared_ptr<BrainTree::BehaviorTree> GetBT() { return mTree; }
		std::shared_ptr<BrainTree::Blackboard> GetBlackBoard() { return mBlackBoard; }

		bool isSuccess() const { return mTree->isSuccess(); }
		bool isFailure() const { return mTree->isFailure(); }
		bool isRunning() const { return mTree->isRunning(); }
		bool isTerminated() const { return mTree->isTerminated(); }

		void reset() { mTree->reset(); }

	protected:


	private:
		std::shared_ptr<BrainTree::BehaviorTree> mTree;
		std::shared_ptr<BrainTree::Blackboard> mBlackBoard;
	};
}
