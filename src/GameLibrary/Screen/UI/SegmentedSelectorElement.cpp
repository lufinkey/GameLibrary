
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
		selectedItemIndex(NO_SELECTION),
		color(Colors::GRAY),
		font(Graphics::getDefaultFont()),
		fontSize(16)
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
		for(auto itemElement : itemElements)
		{
			itemElement->removeFromParentElement();
			delete itemElement;
		}
		itemElements.clear();
		itemElements.reserve(items.size());
		size_t itemIndex = 0;

		double segmentRatio = 1.0/(double)items.size();
		for(auto& item : items)
		{
			auto itemElement = new ButtonElement();
			itemElement->setBorderWidth(1);
			itemElement->setBorderColor(color);
			itemElement->getTitleElement()->setFont(font);
			itemElement->getTitleElement()->setFontSize(fontSize);
			itemElement->setTitle(item, ButtonElement::BUTTONSTATE_NORMAL);
			itemElement->setTitleColor(Colors::BLACK, ButtonElement::BUTTONSTATE_NORMAL);
			if(itemIndex==selectedItemIndex)
			{
				itemElement->setBackgroundColor(color, ButtonElement::BUTTONSTATE_NORMAL);
			}
			else
			{
				itemElement->setBackgroundColor(Colors::WHITE, ButtonElement::BUTTONSTATE_NORMAL);
			}
			itemElement->setTapHandler([=]{
				setSelectedItemIndex(itemIndex);
				if(selectionHandler)
				{
					selectionHandler(itemIndex);
				}
			});

			double left = segmentRatio*(double)itemIndex;
			double right = 1.0-(left+segmentRatio);
			if(itemIndex==(items.size()-1))
			{
				right = 0;
			}
			itemElement->setLayoutRule(LAYOUTRULE_LEFT, left, LAYOUTVALUE_RATIO);
			itemElement->setLayoutRule(LAYOUTRULE_RIGHT, right, LAYOUTVALUE_RATIO);
			itemElement->setLayoutRule(LAYOUTRULE_TOP, 0);
			itemElement->setLayoutRule(LAYOUTRULE_BOTTOM, 0);

			addChildElement(itemElement);
			itemElements.add(itemElement);
			itemIndex++;
		}
	}
	
	const ArrayList<String>& SegmentedSelectorElement::getItems() const
	{
		return items;
	}
	
	void SegmentedSelectorElement::setSelectedItemIndex(size_t index)
	{
		if(selectedItemIndex < items.size())
		{
			itemElements[selectedItemIndex]->setBackgroundColor(Colors::WHITE, ButtonElement::BUTTONSTATE_NORMAL);
		}
		selectedItemIndex = index;
		if(selectedItemIndex < items.size())
		{
			itemElements[selectedItemIndex]->setBackgroundColor(color, ButtonElement::BUTTONSTATE_NORMAL);
		}
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

	void SegmentedSelectorElement::setColor(const Color& color_arg)
	{
		color = color_arg;
		size_t itemIndex = 0;
		for(auto itemElement : itemElements)
		{
			itemElement->setBorderColor(color);
			if(itemIndex==selectedItemIndex)
			{
				itemElement->setBackgroundColor(color, ButtonElement::BUTTONSTATE_NORMAL);
			}
			itemIndex++;
		}
	}

	const Color& SegmentedSelectorElement::getColor() const
	{
		return color;
	}

	void SegmentedSelectorElement::setFont(Font* font_arg)
	{
		font = font_arg;
		for(auto itemElement : itemElements)
		{
			itemElement->getTitleElement()->setFont(font);
		}
	}

	Font* SegmentedSelectorElement::getFont() const
	{
		return font;
	}

	void SegmentedSelectorElement::setFontSize(unsigned int fontSize_arg)
	{
		fontSize = fontSize_arg;
		for(auto itemElement : itemElements)
		{
			itemElement->getTitleElement()->setFontSize(fontSize);
		}
	}

	unsigned int SegmentedSelectorElement::getFontSize() const
	{
		return fontSize;
	}
}
