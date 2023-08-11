#pragma once
#include "guiChild.h"
#include "guiTreeWidget.h"

#include <Engine/IRes.h>
#include <Engine/ResMgr.h>

namespace gui
{
	class guiResources : public guiWindow
	{
	public:
		guiResources();
		virtual ~guiResources();

		virtual void Init() override;

		void ResetContent();

	private:
		template <typename T>
		void AddResources(TreeWidget::tNode* rootNode, const char* name)
		{
			const std::unordered_map<std::string, std::shared_ptr<mh::IRes>, mh::define::tUmap_StringViewHasher, std::equal_to<>>& resources
				= mh::ResMgr::GetResources<T>();

			TreeWidget::tNode* stemNode
				= mTreeWidget->AddNode(rootNode, name, mh::define::tDataPtr{}, true);

			for (const auto& resource : resources)
			{
				mh::define::tDataPtr data{};
				data.SetDataPtr(resource.second.get());
				mTreeWidget->AddNode(stemNode, resource.first, data);
			}
		}

		void toInspector(mh::define::tDataPtr _data);

		TreeWidget* mTreeWidget;
	};
}
