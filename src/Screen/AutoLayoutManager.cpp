
#include <GameLibrary/Screen/AutoLayoutManager.hpp>
#include <GameLibrary/Screen/AutoLayoutCalculator.hpp>
#include <GameLibrary/Utilities/Number.hpp>

namespace fgl
{
	String LayoutRuleType_toString(const LayoutRuleType& layoutRuleType)
	{
		switch(layoutRuleType)
		{
			case LAYOUTRULE_LEFT:
			return "LAYOUTRULE_LEFT";

			case LAYOUTRULE_TOP:
			return "LAYOUTRULE_TOP";
			
			case LAYOUTRULE_RIGHT:
			return "LAYOUTRULE_RIGHT";
			
			case LAYOUTRULE_BOTTOM:
			return "LAYOUTRULE_BOTTOM";
			
			case LAYOUTRULE_CENTER_X:
			return "LAYOUTRULE_CENTER_X";
			
			case LAYOUTRULE_CENTER_Y:
			return "LAYOUTRULE_CENTER_Y";
			
			case LAYOUTRULE_WIDTH:
			return "LAYOUTRULE_WIDTH";
			
			case LAYOUTRULE_HEIGHT:
			return "LAYOUTRULE_HEIGHT";
			
			case LAYOUTRULE_ASPECTRATIO:
			return "LAYOUTRULE_ASPECTRATIO";
		}
		return "";
	}
	
	bool LayoutRuleType_fromString(const String& string, LayoutRuleType* ruleType)
	{
		if(ruleType==nullptr)
		{
			throw IllegalArgumentException("ruleType", "cannot be null");
		}
		if(string.equals("LAYOUTRULE_LEFT"))
		{
			*ruleType = LAYOUTRULE_LEFT;
			return true;
		}
		else if(string.equals("LAYOUTRULE_TOP"))
		{
			*ruleType = LAYOUTRULE_TOP;
			return true;
		}
		else if(string.equals("LAYOUTRULE_RIGHT"))
		{
			*ruleType = LAYOUTRULE_RIGHT;
			return true;
		}
		else if(string.equals("LAYOUTRULE_BOTTOM"))
		{
			*ruleType = LAYOUTRULE_BOTTOM;
			return true;
		}
		else if(string.equals("LAYOUTRULE_CENTER_X"))
		{
			*ruleType = LAYOUTRULE_CENTER_X;
			return true;
		}
		else if(string.equals("LAYOUTRULE_CENTER_Y"))
		{
			*ruleType = LAYOUTRULE_CENTER_Y;
			return true;
		}
		else if(string.equals("LAYOUTRULE_WIDTH"))
		{
			*ruleType = LAYOUTRULE_WIDTH;
			return true;
		}
		else if(string.equals("LAYOUTRULE_HEIGHT"))
		{
			*ruleType = LAYOUTRULE_HEIGHT;
			return true;
		}
		else if(string.equals("LAYOUTRULE_ASPECTRATIO"))
		{
			*ruleType = LAYOUTRULE_ASPECTRATIO;
			return true;
		}
		return false;
	}
	
	String LayoutValueType_toString(const LayoutValueType& layoutValueType)
	{
		switch(layoutValueType)
		{
			case LAYOUTVALUE_PIXEL:
			return "LAYOUTVALUE_PIXEL";
			
			case LAYOUTVALUE_RATIO:
			return "LAYOUTVALUE_RATIO";
		}
		return "";
	}
	
	bool LayoutValueType_fromString(const String& string, LayoutValueType* valueType)
	{
		if(valueType==nullptr)
		{
			throw IllegalArgumentException("valueType", "cannot be null");
		}
		if(string.equals("LAYOUTVALUE_PIXEL"))
		{
			*valueType = LAYOUTVALUE_PIXEL;
			return true;
		}
		else if(string.equals("LAYOUTVALUE_RATIO"))
		{
			*valueType = LAYOUTVALUE_RATIO;
			return true;
		}
		return false;
	}
	
	AutoLayoutManager::AutoLayoutManager() : containerOffset(false)
	{
		//
	}
	
	AutoLayoutManager::AutoLayoutManager(const AutoLayoutManager& autoLayoutMgr)
	{
		rules = autoLayoutMgr.rules;
		containerOffset = autoLayoutMgr.containerOffset;
	}
	
	AutoLayoutManager& AutoLayoutManager::operator=(const AutoLayoutManager& autoLayoutMgr)
	{
		rules = autoLayoutMgr.rules;
		containerOffset = autoLayoutMgr.containerOffset;
		return *this;
	}
	
	void AutoLayoutManager::setRule(const LayoutRuleType& ruleType, double value, const LayoutValueType& valueType)
	{
		for(size_t rules_size=rules.size(), i=0; i<rules_size; i++)
		{
			if(rules[i].ruleType==ruleType)
			{
				rules.remove(i);
				i = rules_size;
			}
		}
		LayoutRule rule;
		rule.ruleType = ruleType;
		rule.value = value;
		rule.valueType = valueType;
		rules.add(rule);
	}
	
	void AutoLayoutManager::setRules(const Dictionary& rulesDict)
	{
		removeAllRules();
		const ArrayList<std::pair<String, Any> >& contents = rulesDict.getContents();
		for(size_t contents_size=contents.size(), i=0; i<contents_size; i++)
		{
			const std::pair<String, Any> pair = contents.get(i);
			LayoutRuleType ruleType;
			if(LayoutRuleType_fromString(pair.first, &ruleType))
			{
				if(pair.second.is<Dictionary>())
				{
					const Dictionary& ruleData = pair.second.as<const Dictionary&>();
					Any value_any = ruleData.get("value", Any());
					if(!value_any.isEmpty() && value_any.is<Number>())
					{
						double value = value_any.as<Number>().toArithmeticValue<double>();
						LayoutValueType valueType = LAYOUTVALUE_PIXEL;
						Any valueType_any = ruleData.get("valueType", Any());
						if(!valueType_any.isEmpty() && valueType_any.is<String>())
						{
							String valueType_str = valueType_any.as<String>();
							LayoutValueType_fromString(valueType_str, &valueType);
						}
						setRule(ruleType, value, valueType);
					}
				}
			}
		}
	}
	
	bool AutoLayoutManager::hasRules() const
	{
		if(rules.size()>0)
		{
			return true;
		}
		return false;
	}
	
	LayoutRule* AutoLayoutManager::getRule(const LayoutRuleType& ruleType) const
	{
		for(size_t rules_size=rules.size(), i=0; i<rules_size; i++)
		{
			LayoutRule* rule = (LayoutRule*)&rules[i];
			if(rule->ruleType==ruleType)
			{
				return rule;
			}
		}
		return nullptr;
	}
	
	void AutoLayoutManager::setOffsetByContainer(bool toggle)
	{
		containerOffset = toggle;
	}
	
	bool AutoLayoutManager::isOffsetByContainer() const
	{
		return containerOffset;
	}
	
	void AutoLayoutManager::removeAllRules()
	{
		rules.clear();
	}
	
	RectangleD AutoLayoutManager::calculateFrame(const RectangleD& currentFrame, const RectangleD& containerFrame) const
	{
		AutoLayoutCalculator layoutCalc(currentFrame, containerFrame);
		for(size_t rules_size=rules.size(), i=0; i<rules_size; i++)
		{
			const LayoutRule& rule = rules[i];
			switch(rule.ruleType)
			{
				case LAYOUTRULE_LEFT:
				layoutCalc.setLeft(rule.value, rule.valueType);
				break;
				
				case LAYOUTRULE_TOP:
				layoutCalc.setTop(rule.value, rule.valueType);
				break;
				
				case LAYOUTRULE_RIGHT:
				layoutCalc.setRight(rule.value, rule.valueType);
				break;
				
				case LAYOUTRULE_BOTTOM:
				layoutCalc.setBottom(rule.value, rule.valueType);
				break;
				
				case LAYOUTRULE_CENTER_X:
				layoutCalc.setCenterX(rule.value, rule.valueType);
				break;
				
				case LAYOUTRULE_CENTER_Y:
				layoutCalc.setCenterY(rule.value, rule.valueType);
				break;
				
				case LAYOUTRULE_WIDTH:
				layoutCalc.setWidth(rule.value, rule.valueType);
				break;
				
				case LAYOUTRULE_HEIGHT:
				layoutCalc.setHeight(rule.value, rule.valueType);
				break;
				
				case LAYOUTRULE_ASPECTRATIO:
				layoutCalc.setAspectRatio(rule.value);
				break;
			}
		}
		if(containerOffset)
		{
			RectangleD calculatedFrame = layoutCalc.getCalculatedFrame();
			calculatedFrame.x += containerFrame.x;
			calculatedFrame.y += containerFrame.y;
			return calculatedFrame;
		}
		return layoutCalc.getCalculatedFrame();
	}
}
