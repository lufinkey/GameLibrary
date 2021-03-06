
#pragma once

#include <GameLibrary/Utilities/Geometry/Rectangle.hpp>
#include "AutoLayoutManager.hpp"

namespace fgl
{
	class AutoLayoutCalculator
	{
	public:
		AutoLayoutCalculator(const RectangleD& currentFrame, const Vector2d& containerSize);
		
		RectangleD getCalculatedFrame() const;
		
		void setLeft(double value, const LayoutValueType& valueType);
		void setTop(double value, const LayoutValueType& valueType);
		void setRight(double value, const LayoutValueType& valueType);
		void setBottom(double value, const LayoutValueType& valueType);
		
		void setCenterX(double value, const LayoutValueType& valueType);
		void setCenterY(double value, const LayoutValueType& valueType);
		
		void setWidth(double value, const LayoutValueType& valueType);
		void setHeight(double value, const LayoutValueType& valueType);
		
		void setAspectRatio(double value);
		
	private:
		class LayoutValue
		{
		public:
			LayoutValue();
			LayoutValue(const double& value);
			LayoutValue(const LayoutValue&);
			
			LayoutValue& operator=(const LayoutValue&);
			LayoutValue& operator=(const double& value);
			
			operator double() const;
			double getValue() const;
			
			void set();
			void unset();
			bool isSet() const;
			
		private:
			double value;
			bool isset;
		};
		
		double resolveValueX(double value, const LayoutValueType& valueType) const;
		double resolveValueY(double value, const LayoutValueType& valueType) const;
		
		void layoutAspectRatioFromWidth();
		void layoutAspectRatioFromHeight();
		
		fgl::Vector2d containerSize;
		
		LayoutValue beginX;
		LayoutValue beginY;
		LayoutValue endX;
		LayoutValue endY;
		
		LayoutValue centerX;
		LayoutValue centerY;
		
		LayoutValue width;
		LayoutValue height;
		
		LayoutValue aspectRatio;
	};
}
