
#pragma once

#include <GameLibrary/Utilities/Geometry/Rectangle.hpp>
#include <GameLibrary/Utilities/ArrayList.hpp>
#include <GameLibrary/Utilities/Dictionary.hpp>
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	enum LayoutRuleType
	{
		LAYOUTRULE_LEFT,
		LAYOUTRULE_TOP,
		LAYOUTRULE_RIGHT,
		LAYOUTRULE_BOTTOM,
		LAYOUTRULE_CENTER_X,
		LAYOUTRULE_CENTER_Y,
		LAYOUTRULE_WIDTH,
		LAYOUTRULE_HEIGHT,
		//! width/height
		LAYOUTRULE_ASPECTRATIO
	};
	String LayoutRuleType_toString(const LayoutRuleType& layoutRuleType);
	bool LayoutRuleType_fromString(const String& string, LayoutRuleType* ruleType);
	
	enum LayoutValueType
	{
		LAYOUTVALUE_PIXEL,
		LAYOUTVALUE_RATIO
	};
	String LayoutValueType_toString(const LayoutValueType& layoutValueType);
	bool LayoutValueType_fromString(const String& string, LayoutValueType* valueType);
	
	struct LayoutRule
	{
		LayoutRuleType ruleType;
		double value;
		LayoutValueType valueType;
	};
	
	/*! Automatically lays out Actor and ScreenElement objects based on a given frame*/
	class AutoLayoutManager
	{
	public:
		/*! default constructor*/
		AutoLayoutManager();
		/*! copy constructor*/
		AutoLayoutManager(const AutoLayoutManager&);
		/*! assignment operator*/
		AutoLayoutManager& operator=(const AutoLayoutManager&);
		
		void setRule(const LayoutRuleType& ruleType, double value, const LayoutValueType& valueType=LAYOUTVALUE_PIXEL);
		void setRules(const Dictionary& rules);
		bool hasRules() const;
		LayoutRule* getRule(const LayoutRuleType& ruleType) const;
		void setOffsetByContainer(bool toggle);
		bool isOffsetByContainer() const;

		void removeAllRules();
		
		RectangleD calculateFrame(const RectangleD& currentFrame, const RectangleD& containerFrame) const;
		
	private:
		ArrayList<LayoutRule> rules;
		bool containerOffset;
	};
}

REGISTER_TYPE(fgl, AutoLayoutManager)
