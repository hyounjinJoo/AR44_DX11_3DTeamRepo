#pragma once
#include "guiChild.h"
#include "guiTreeWidget.h"

#include <Engine/IRes.h>
#include <Engine/ResMgr.h>

namespace gui
{
	class Project : public guiChild
	{
	public:
		Project();
		virtual ~Project();


		//virtual void Update() override;

		void ResetContent();

	private:
		template <typename T>
		void AddResources(TreeWidget::tNode* rootNode, const char* name)
		{
			const std::unordered_map<std::string, std::shared_ptr<mh::IRes>, mh::define::tUmap_StringViewHasher, std::equal_to<>>& resources
				= mh::ResMgr::GetResources<T>();

			TreeWidget::tNode* stemNode
				= mTreeWidget->AddNode(rootNode, name, tData{}, true);

			for (const auto& resource : resources)
			{
				tData data{};
				data.SetDataPtr(resource.second.get());
				mTreeWidget->AddNode(stemNode, resource.first, data);
			}
		}

		void toInspector(tData _data);

		TreeWidget* mTreeWidget;
	};
}
