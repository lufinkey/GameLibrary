
#pragma once

#include "ZoomPanElement.hpp"
#include "ButtonElement.hpp"

namespace fgl
{
	class GridSelectorElement : public fgl::ZoomPanElement
	{
	public:
		static const size_t NO_SELECTION = -1;
		
		GridSelectorElement();
		explicit GridSelectorElement(const fgl::RectangleD& frame);
		virtual ~GridSelectorElement();
		
		virtual void layoutChildElements() override;
		virtual void update(fgl::ApplicationData appData) override;
		
		void setItems(const fgl::ArrayList<fgl::ButtonElement*>& items);
		const fgl::ArrayList<fgl::ButtonElement*>& getItems() const;
		
		void setItemSize(const fgl::Vector2d& itemSize);
		const fgl::Vector2d& getItemSize() const;
		
		void setSelectedItemIndex(size_t index);
		size_t getSelectedItemIndex() const;
		
		void setSelectionChangeHandler(const std::function<void()>& func);
		const std::function<void()>& getSelectionChangeHandler() const;
		
	private:
		fgl::ArrayList<fgl::ButtonElement*> items;
		fgl::Vector2d itemSize;
		fgl::Vector2d itemSpacing;
		fgl::ArrayList<fgl::ButtonElement*> pressedItems;
		size_t selectedItemIndex;
		std::function<void()> onSelectionChange;
	};
}

REGISTER_TYPE(fgl::GridSelectorElement, fgl::ZoomPanElement)
