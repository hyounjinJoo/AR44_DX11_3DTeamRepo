#pragma once
#include "guiWidget.h"
#include "guiTreeWidget.h"

#include <Engine/IRes.h>
#include <Engine/ResMgr.h>

namespace gui
{
	class Project : public Widget
	{
	public:
		Project();
		~Project();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void ResetContent();

	private:
		template <typename T>
		void AddResources(TreeWidget::tNode* rootNode, const char* name)
		{
			const std::unordered_map<std::string, std::shared_ptr<mh::IRes>, mh::define::tUmap_StringViewHasher, std::equal_to<>>& resources
				= mh::ResMgr::GetInst()->GetResources<T>();

			TreeWidget::tNode* stemNode
				= mTreeWidget->AddNode(rootNode, name, 0, true);

			for (const auto& resource : resources)
			{
				mTreeWidget->AddNode(stemNode, resource.first, resource.second.get());
			}
		}

		void toInspector(void* data);

		TreeWidget* mTreeWidget;
	};
}
