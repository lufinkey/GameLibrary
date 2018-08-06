
#include <GameLibrary/Screen/UI/GridSelectorElement.hpp>

namespace fgl
{
	GridSelectorElement::GridSelectorElement()
		: GridSelectorElement(fgl::RectangleD(0,0,300,200))
	{
		//
	}
	
	GridSelectorElement::GridSelectorElement(const fgl::RectangleD& frame)
		: ZoomPanElement(frame),
		itemSize(100, 100),
		itemSpacing(10, 10),
		selectedItemIndex(-1)
	{
		//
	}
	
	void GridSelectorElement::update(fgl::ApplicationData appData)
	{
		ZoomPanElement::update(appData);
		
		size_t prevSelectedItemIndex = selectedItemIndex;
		for(size_t i=0; i<items.size(); i++)
		{
			auto& item = items.get(i);
			if(item->isPressed())
			{
				if(!pressedItems.contains(item))
				{
					pressedItems.add(item);
					if(prevSelectedItemIndex==selectedItemIndex)
					{
						selectedItemIndex = i;
					}
				}
			}
			else
			{
				pressedItems.removeEqual(item);
			}
		}
		
		for(size_t i=0; i<items.size(); i++)
		{
			auto& item = items[i];
			if(i==selectedItemIndex)
			{
				item->setSelected(true);
			}
			else
			{
				item->setSelected(false);
			}
		}
		
		if(prevSelectedItemIndex != selectedItemIndex)
		{
			if(onSelectionChange)
			{
				onSelectionChange();
			}
		}
	}
	
	void GridSelectorElement::onLayoutChildElements()
	{
		ZoomPanElement::onLayoutChildElements();
		
		if(items.size() > 0)
		{
			auto frame = getFrame();
			auto cols = (unsigned int)(frame.width/(itemSize.x+itemSpacing.x));
			auto extraSpace = frame.width - ((itemSize.x+itemSpacing.x)*(double)cols);
			auto spacing = extraSpace / (double)(cols+1);
			auto objX = (itemSpacing.x/2.0)+spacing;
			auto objY = (itemSpacing.y/2.0);
			unsigned int colCounter = 0;
			for(auto item : items)
			{
				item->setFrame(fgl::RectangleD(objX+(itemSpacing.x/2.0), objY, 100.0, 100.0));
				
				objX += itemSize.x + itemSpacing.x + spacing;
				colCounter++;
				if(colCounter >= cols)
				{
					colCounter = 0;
					objY += itemSize.y + itemSpacing.y;
				}
			}
			if(colCounter!=0)
			{
				objY += itemSize.y + itemSpacing.y;
			}
			setContentSize(fgl::Vector2d(frame.width, objY));
		}
	}
	
	void GridSelectorElement::setItems(const fgl::ArrayList<fgl::ButtonElement*>& items_args)
	{
		for(auto& item : items) {
			item->removeFromParentElement();
		}
		items = items_args;
		for(auto& item : items) {
			addChildElement(item);
		}
		layoutChildElements();
	}
	
	const fgl::ArrayList<fgl::ButtonElement*>& GridSelectorElement::getItems() const
	{
		return items;
	}
	
	void GridSelectorElement::setItemSize(const fgl::Vector2d& itemSize_arg)
	{
		itemSize = itemSize_arg;
		layoutChildElements();
	}
	
	const fgl::Vector2d& GridSelectorElement::getItemSize() const
	{
		return itemSize;
	}
	
	void GridSelectorElement::setSelectedItemIndex(size_t index)
	{
		selectedItemIndex = index;
	}
	
	size_t GridSelectorElement::getSelectedItemIndex() const
	{
		return selectedItemIndex;
	}
	
	void GridSelectorElement::setSelectionChangeHandler(const std::function<void()>& func)
	{
		onSelectionChange = func;
	}
	
	const std::function<void()>& GridSelectorElement::getSelectionChangeHandler() const
	{
		return onSelectionChange;
	}
}
