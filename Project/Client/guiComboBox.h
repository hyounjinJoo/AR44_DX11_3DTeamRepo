#pragma once
#include "guiBase.h"
#include <Engine/define_Struct.h>

namespace gui
{
	class guiComboBox :
		public guiBase
	{
	public:
		struct tComboItem
		{
			std::string strName;
			mh::define::tDataPtr pData;

			tComboItem() : strName(), pData() {};

			tComboItem(const std::string& _strName, mh::define::tDataPtr _pData = {})
				: strName(_strName)
				, pData(_pData)
			{}
		};

		guiComboBox();
		virtual ~guiComboBox();

		virtual bool BeginUI() override;
		virtual void UpdateUI() override;
		virtual void EndUI() override;

		void SetImguiComboFlag(ImGuiComboFlags _Flags) { mComboFlags = _Flags; }

		void AddItem(const tComboItem& _tItem) { mItems.push_back(_tItem); }

		//새로 들어온 벡터값으로 값을 교체
		void SetItems(std::vector<tComboItem>& _vecItem) { Reset(); std::swap(mItems, _vecItem); }

		void Reset() { mItems.clear(); mCurrentSelected = -1; mComboFlags = 0; }

		size_t GetItemCount() const { return mItems.size(); }

		void SetCurrentIndex(int _idx) { mCurrentSelected = _idx; }
		int	GetCurrentIndex() const { return mCurrentSelected; }
		void SetCurrentSelected(const std::string& _SelectedName);
		const tComboItem& GetCurrentSelected() const;
		void UnSelect() { mCurrentSelected = -1; }

		void SetClickCallback(std::function<void(const tComboItem&)> _pFunc) { mCallback = _pFunc; }

	private:
		bool IsIndexValid() const;

	private:
		const static tComboItem gVoidItem;

		std::vector<tComboItem> mItems;
		int mCurrentSelected;
		ImGuiComboFlags mComboFlags;

		//함수 호출 대상
		std::function<void(const tComboItem&)> mCallback;
	};

	inline bool guiComboBox::IsIndexValid() const
	{
		return (0 <= mCurrentSelected && mCurrentSelected < mItems.size());
	}
}


