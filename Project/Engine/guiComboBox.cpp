#include "PCH_Engine.h"
#include "guiComboBox.h"


namespace gui
{
	const guiComboBox::tComboItem guiComboBox::gVoidItem{};

	guiComboBox::guiComboBox()
		: guiBase("ComboBox")
		, mItems()
		, mCurrentSelected(-1)
		, mComboFlags()
		, mCallback()
		, mbSelectionChanged()
	{

	}
	guiComboBox::~guiComboBox()
	{
	}
	bool guiComboBox::BeginUI()
	{
		mbSelectionChanged = false;

		std::string Preview;
		if (IsIndexValid())
			Preview = mItems[mCurrentSelected].strName;

		return ImGui::BeginCombo(GetKey().c_str(), Preview.c_str(), mComboFlags);
	}
	void guiComboBox::UpdateUI()
	{
		

		for (int n = 0; n < (int)mItems.size(); ++n)
		{
			bool isSelected = (mCurrentSelected == n);

			//여기는 콤보박스에서 특정 값이 선택(클릭)되었을 떄 진입한다.
			if (true == ImGui::Selectable(mItems[n].strName.c_str(), isSelected))
			{
				mCurrentSelected = n;
				mbSelectionChanged = true;

				if (mCallback)
					mCallback(mItems[n]);
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
	}
	void guiComboBox::EndUI()
	{
		ImGui::EndCombo();
	}

	void guiComboBox::SetCurrentSelected(const std::string& _SelectedName)
	{
		size_t size = mItems.size();
		for (size_t i = 0; i < size; i++)
		{
			if (_SelectedName == mItems[i].strName)
			{
				mCurrentSelected = (int)i;
				return;
			}
		}

		mCurrentSelected = -1;
	}

	const guiComboBox::tComboItem& guiComboBox::GetCurrentSelected() const
	{
		if (true == IsIndexValid())
			return mItems[mCurrentSelected];
		else
			return gVoidItem;
	}


}
