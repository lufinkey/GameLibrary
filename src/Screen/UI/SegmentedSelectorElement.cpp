
#include <GameLibrary/Screen/UI/SegmentedSelectorElement.hpp>

namespace fgl
{
	const size_t SegmentedSelectorElement::NO_SELECTION;
	
	SegmentedSelectorElement::SegmentedSelectorElement() : SegmentedSelectorElement(RectangleD(0,0,0,0))
	{
		//
	}
	
	SegmentedSelectorElement::SegmentedSelectorElement(const RectangleD& frame)
		: ScreenElement(frame),
		selectedItemIndex(NO_SELECTION)
	{
		//
	}
	
	SegmentedSelectorElement::~SegmentedSelectorElement()
	{
		for(auto itemElement : itemElements)
		{
			delete itemElement;
		}
	}
	
	void SegmentedSelectorElement::setItems(const ArrayList<String>& items_arg)
	{
		items = items_arg;
		//TODO update itemElements
	}
	
	const ArrayList<String>& SegmentedSelectorElement::getItems() const
	{
		return items;
	}
	
	void SegmentedSelectorElement::setSelectedItemIndex(size_t index)
	{
		selectedItemIndex = index;
		//TODO update itemElement states
	}
	
	size_t SegmentedSelectorElement::getSelectedItemIndex() const
	{
		return selectedItemIndex;
	}
	
	void SegmentedSelectorElement::setSelectionHandler(const std::function<void(size_t)>& handler)
	{
		selectionHandler = handler;
	}
	
	const std::function<void(size_t)>& SegmentedSelectorElement::getSelectionHandler() const
	{
		return selectionHandler;
	}
}
