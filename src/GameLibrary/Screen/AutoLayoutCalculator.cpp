
#include <GameLibrary/Screen/AutoLayoutCalculator.hpp>

namespace fgl
{
	AutoLayoutCalculator::AutoLayoutCalculator(const RectangleD& currentFrame, const RectangleD& containerFrame)
		: containerFrame(containerFrame),
		beginX(currentFrame.x),
		beginY(currentFrame.y),
		endX(currentFrame.x+currentFrame.width),
		endY(currentFrame.y+currentFrame.height),
		centerX(currentFrame.x+(currentFrame.width/2)),
		centerY(currentFrame.y+(currentFrame.height/2)),
		width(currentFrame.width),
		height(currentFrame.height),
		aspectRatio(currentFrame.height!=0 ? (currentFrame.width/currentFrame.height) : 0)
	{
		//
	}
	
	RectangleD AutoLayoutCalculator::getCalculatedFrame() const
	{
		return RectangleD(beginX, beginY, endX-beginX, endY-beginY);
	}
	
	void AutoLayoutCalculator::setLeft(double value, const LayoutValueType& valueType)
	{
		if(beginX.isSet())
		{
			return;
		}
		
		beginX = resolveValueX(value, valueType);
		beginX.set();
		
		if(endX.isSet())
		{
			width = endX - beginX;
			width.set();
			centerX = beginX + (width/2);
			centerX.set();
		}
		else if(width.isSet())
		{
			endX = beginX + width;
			endX.set();
			centerX = beginX + (width/2);
			centerX.set();
		}
		else if(centerX.isSet())
		{
			width = 2*(centerX-beginX);
			width.set();
			endX = beginX + width;
			endX.set();
		}
		else
		{
			endX = beginX + width;
			centerX = beginX + (width/2);
		}
		if(aspectRatio.isSet())
		{
			if(width.isSet())
			{
				layoutAspectRatioFromWidth();
			}
			else if(height.isSet())
			{
				layoutAspectRatioFromHeight();
			}
		}
	}
	
	void AutoLayoutCalculator::setTop(double value, const LayoutValueType& valueType)
	{
		if(beginY.isSet())
		{
			return;
		}

		beginY = resolveValueY(value, valueType);
		beginY.set();

		if(endY.isSet())
		{
			height = endY - beginY;
			height.set();
			centerY = beginY + (height/2);
			centerY.set();
		}
		else if(height.isSet())
		{
			endY = beginY + height;
			endY.set();
			centerY = beginY + (height/2);
			centerY.set();
		}
		else if(centerY.isSet())
		{
			height = 2*(centerY-beginY);
			height.set();
			endY = beginY + height;
			endY.set();
		}
		else
		{
			endY = beginY + height;
			centerY = beginY + (height/2);
		}
		if(aspectRatio.isSet())
		{
			if(height.isSet())
			{
				layoutAspectRatioFromHeight();
			}
			else if(width.isSet())
			{
				layoutAspectRatioFromWidth();
			}
		}
	}
	
	void AutoLayoutCalculator::setRight(double value, const LayoutValueType& valueType)
	{
		if(endX.isSet())
		{
			return;
		}

		endX = containerFrame.width-resolveValueX(value, valueType);
		endX.set();

		if(beginX.isSet())
		{
			width = endX - beginX;
			width.set();
			centerX = beginX + (width/2);
			centerX.set();
		}
		else if(width.isSet())
		{
			beginX = endX - width;
			beginX.set();
			centerX = beginX + (width/2);
			centerX.set();
		}
		else if(centerX.isSet())
		{
			width = 2*(endX-centerX);
			width.set();
			beginX = endX - width;
			beginX.set();
		}
		else
		{
			beginX = endX - width;
			centerX = beginX + (width/2);
		}
		if(aspectRatio.isSet())
		{
			if(width.isSet())
			{
				layoutAspectRatioFromWidth();
			}
			else if(height.isSet())
			{
				layoutAspectRatioFromHeight();
			}
		}
	}
	
	void AutoLayoutCalculator::setBottom(double value, const LayoutValueType& valueType)
	{
		if(endY.isSet())
		{
			return;
		}

		endY = containerFrame.height-resolveValueY(value, valueType);
		endY.set();

		if(beginY.isSet())
		{
			height = endY - beginY;
			height.set();
			centerY = beginY + (height/2);
			centerY.set();
		}
		else if(height.isSet())
		{
			beginY = endY - height;
			beginY.set();
			centerY = beginY + (height/2);
			centerY.set();
		}
		else if(centerY.isSet())
		{
			height = 2*(endY-centerY);
			height.set();
			beginY = endY - height;
			beginY.set();
		}
		else
		{
			beginY = endY - height;
			centerY = beginY + (height/2);
		}
		if(aspectRatio.isSet())
		{
			if(height.isSet())
			{
				layoutAspectRatioFromHeight();
			}
			else if(width.isSet())
			{
				layoutAspectRatioFromWidth();
			}
		}
	}
	
	void AutoLayoutCalculator::setCenterX(double value, const LayoutValueType& valueType)
	{
		if(centerX.isSet())
		{
			return;
		}
		
		centerX = resolveValueX(value, valueType);
		centerX.set();

		if(beginX.isSet())
		{
			endX = centerX + (centerX-beginX);
			endX.set();
			width = endX - beginX;
			width.set();
		}
		else if(endX.isSet())
		{
			beginX = centerX - (endX-centerX);
			beginX.set();
			width = endX - beginX;
			width.set();
		}
		else if(width.isSet())
		{
			beginX = centerX - (width/2);
			beginX.set();
			endX = beginX + width;
			endX.set();
		}
		else
		{
			beginX = centerX - (width/2);
			endX = beginX + width;
		}
		if(aspectRatio.isSet())
		{
			if(width.isSet())
			{
				layoutAspectRatioFromWidth();
			}
			else if(height.isSet())
			{
				layoutAspectRatioFromHeight();
			}
		}
	}
	
	void AutoLayoutCalculator::setCenterY(double value, const LayoutValueType& valueType)
	{
		if(centerY.isSet())
		{
			return;
		}

		centerY = resolveValueY(value, valueType);
		centerY.set();

		if(beginY.isSet())
		{
			endY = centerY + (centerY-beginY);
			endY.set();
			height = endY - beginY;
			height.set();
		}
		else if(endY.isSet())
		{
			beginY = centerY - (endY-centerY);
			beginY.set();
			height = endY - beginY;
			height.set();
		}
		else if(height.isSet())
		{
			beginY = centerY - (height/2);
			beginY.set();
			endY = beginY + height;
			endY.set();
		}
		else
		{
			beginY = centerY - (height/2);
			endY = beginY + height;
		}
		if(aspectRatio.isSet())
		{
			if(height.isSet())
			{
				layoutAspectRatioFromHeight();
			}
			else if(width.isSet())
			{
				layoutAspectRatioFromWidth();
			}
		}
	}
	
	void AutoLayoutCalculator::setWidth(double value, const LayoutValueType& valueType)
	{
		if(width.isSet())
		{
			return;
		}
		
		width = resolveValueX(value, valueType);
		width.set();
		
		if(beginX.isSet())
		{
			endX = beginX + width;
			endX.set();
			centerX = beginX + (width/2);
			centerX.set();
		}
		else if(endX.isSet())
		{
			beginX = endX - width;
			beginX.set();
			centerX = beginX + (width/2);
			centerX.set();
		}
		else if(centerX.isSet())
		{
			beginX = centerX - (width/2);
			beginX.set();
			endX = beginX + width;
			endX.set();
		}
		else
		{
			endX = beginX + width;
			centerX = beginX + (width/2);
		}
		if(aspectRatio.isSet())
		{
			if(width.isSet())
			{
				layoutAspectRatioFromWidth();
			}
			else if(height.isSet())
			{
				layoutAspectRatioFromHeight();
			}
		}
	}
	
	void AutoLayoutCalculator::setHeight(double value, const LayoutValueType& valueType)
	{
		if(height.isSet())
		{
			return;
		}

		height = resolveValueY(value, valueType);
		height.set();

		if(beginY.isSet())
		{
			endY = beginY + height;
			endY.set();
			centerY = beginY + (height/2);
			centerY.set();
		}
		else if(endY.isSet())
		{
			beginY = endY - height;
			beginY.set();
			centerY = beginY + (height/2);
			centerY.set();
		}
		else if(centerY.isSet())
		{
			beginY = centerY - (height/2);
			beginY.set();
			endY = beginY + height;
			endY.set();
		}
		else
		{
			endY = beginY + height;
			centerY = beginY + (height/2);
		}
		if(aspectRatio.isSet())
		{
			if(height.isSet())
			{
				layoutAspectRatioFromHeight();
			}
			else if(width.isSet())
			{
				layoutAspectRatioFromWidth();
			}
		}
	}
	
	void AutoLayoutCalculator::setAspectRatio(double value)
	{
		if(aspectRatio.isSet())
		{
			return;
		}
		else if(width.isSet() && height.isSet())
		{
			if(height==0)
			{
				aspectRatio = 0;
			}
			else
			{
				aspectRatio = width/height;
			}
			aspectRatio.set();
			return;
		}
		
		aspectRatio = value;
		aspectRatio.set();
		
		if(width.isSet())
		{
			layoutAspectRatioFromWidth();
		}
		else if(height.isSet())
		{
			layoutAspectRatioFromHeight();
		}
	}
	
	double AutoLayoutCalculator::resolveValueX(double value, const LayoutValueType& valueType) const
	{
		switch(valueType)
		{
			case LAYOUTVALUE_PIXEL:
			return value;
			
			case LAYOUTVALUE_RATIO:
			return containerFrame.width*value;
		}
		return value;
	}
	
	double AutoLayoutCalculator::resolveValueY(double value, const LayoutValueType& valueType) const
	{
		switch(valueType)
		{
			case LAYOUTVALUE_PIXEL:
			return value;

			case LAYOUTVALUE_RATIO:
			return containerFrame.height*value;
		}
		return value;
	}
	
	void AutoLayoutCalculator::layoutAspectRatioFromWidth()
	{
		if(aspectRatio==0)
		{
			height = 0;
		}
		else
		{
			height = width/aspectRatio;
		}
		height.set();
		if(beginY.isSet())
		{
			endY = beginY + height;
			endY.set();
			centerY = beginY + (height/2);
			centerY.set();
		}
		else if(endY.isSet())
		{
			beginY = endY - height;
			beginY.set();
			centerY = beginY + (height/2);
			centerY.set();
		}
		else
		{
			endY = beginY + height;
			centerY = beginY + (height/2);
		}
	}
	
	void AutoLayoutCalculator::layoutAspectRatioFromHeight()
	{
		if(aspectRatio==0)
		{
			width = 0;
		}
		else
		{
			width = height*aspectRatio;
		}
		width.set();
		if(beginX.isSet())
		{
			endX = beginX + width;
			endX.set();
			centerX = beginX + (width/2);
			centerX.set();
		}
		else if(endX.isSet())
		{
			beginX = endX - width;
			beginX.set();
			centerX = beginX + (width/2);
			centerX.set();
		}
		else
		{
			endX = beginX + width;
			centerX = beginX + (width/2);
		}
	}
	
	AutoLayoutCalculator::LayoutValue::LayoutValue() : value(0), isset(false) {}
	AutoLayoutCalculator::LayoutValue::LayoutValue(const double& value) : value(value), isset(false) {}
	AutoLayoutCalculator::LayoutValue::LayoutValue(const LayoutValue& layoutValue) : value(layoutValue.value), isset(layoutValue.isset) {}
	
	AutoLayoutCalculator::LayoutValue& AutoLayoutCalculator::LayoutValue::operator=(const LayoutValue& layoutValue)
	{
		value = layoutValue.value;
		isset = layoutValue.isset;
		return *this;
	}
	
	AutoLayoutCalculator::LayoutValue& AutoLayoutCalculator::LayoutValue::operator=(const double& val)
	{
		value = val;
		return *this;
	}
	
	AutoLayoutCalculator::LayoutValue::operator double() const
	{
		return value;
	}
	
	double AutoLayoutCalculator::LayoutValue::getValue() const
	{
		return value;
	}
	
	void AutoLayoutCalculator::LayoutValue::set()
	{
		isset = true;
	}
	
	void AutoLayoutCalculator::LayoutValue::unset()
	{
		isset = false;
	}
	
	bool AutoLayoutCalculator::LayoutValue::isSet() const
	{
		return isset;
	}
}
