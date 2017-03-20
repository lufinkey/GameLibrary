
#pragma once

#include <GameLibrary/Screen/ScreenElement.hpp>
#include "ButtonElement.hpp"

namespace fgl
{
	class SegmentedSelectorElement : public ScreenElement
	{
	public:
		static const size_t NO_SELECTION = -1;
		
		SegmentedSelectorElement();
		SegmentedSelectorElement(const RectangleD& frame);
		virtual ~SegmentedSelectorElement();
		
		void setItems(const ArrayList<String>& items);
		const ArrayList<String>& getItems() const;
		
		void setSelectedItemIndex(size_t index);
		size_t getSelectedItemIndex() const;
		
		void setSelectionHandler(const std::function<void(size_t index)>& handler);
		const std::function<void(size_t)>& getSelectionHandler() const;
		
	private:
		ArrayList<String> items;
		size_t selectedItemIndex;
		std::function<void(size_t)> selectionHandler;
		
		ArrayList<ButtonElement*> itemElements;
	};
}
