
#include <GameLibrary/Physics/CollisionPair.hpp>

namespace fgl
{
	CollisionPair::CollisionPair(Collidable* collidable1, Collidable* collidable2)
		: collidable1(collidable1), collidable2(collidable2)
	{
		//
	}

	bool CollisionPair::operator==(const CollisionPair& pair) const
	{
		if((collidable1==pair.collidable1 && collidable2==pair.collidable2) || (collidable1==pair.collidable2 && collidable2==pair.collidable1))
		{
			return true;
		}
		return false;
	}

	bool CollisionPair::operator!=(const CollisionPair& pair) const
	{
		return !operator==(pair);
	}

	bool CollisionPair::shouldIgnoreCollision(CollisionRect* rect1, CollisionRect* rect2) const
	{
		auto tag1 = rect1->getTag();
		auto tag2 = rect2->getTag();
		for(auto& tagPair : ignoredRectPairs)
		{
			if(tagPair.first==tag1 && tagPair.second==tag2)
			{
				return true;
			}
		}
		return false;
	}
	
	ArrayList<CollisionRectTagPair> CollisionPair::getContactingRectPairs() const
	{
		return collidedRectPairs.mergedWith(ignoredRectPairs);
	}
	
	ArrayList<CollisionRectTagPair> CollisionPair::getReverseContactingRectPairs() const
	{
		auto contactingRectPairs = getContactingRectPairs();
		auto reverseContactingRectPairs = fgl::ArrayList<CollisionRectTagPair>();
		reverseContactingRectPairs.reserve(contactingRectPairs.size());
		for(auto& pair : contactingRectPairs)
		{
			reverseContactingRectPairs.add(CollisionRectTagPair(pair.second, pair.first));
		}
		return reverseContactingRectPairs;
	}

	ArrayList<CollisionRectPair> CollisionPair::getCollisionRectPairs(const ArrayList<CollisionRect*>& rects1, const ArrayList<CollisionRect*>& rects2) const
	{
		auto priorityRectPairs = getContactingRectPairs();
		size_t pair_count = rects1.size()*rects2.size();
		ArrayList<CollisionRectPair> pairs;
		pairs.reserve(pair_count);
		for(auto& priorityRect : priorityRectPairs)
		{
			size_t rectIndex1 = rects1.indexWhere([&priorityRect](CollisionRect* const & rect) -> bool {
				if(rect->getTag()==priorityRect.first)
				{
					return true;
				}
				return false;
			});
			size_t rectIndex2 = rects2.indexWhere([&priorityRect](CollisionRect* const & rect) -> bool {
				if(rect->getTag()==priorityRect.second)
				{
					return true;
				}
				return false;
			});
			if(rectIndex1!=-1 && rectIndex2!=-1)
			{
				pairs.add(CollisionRectPair(rects1[rectIndex1], rects2[rectIndex2]));
			}
		}
		auto priorityPairs = pairs;
		for(auto& rect1 : rects1)
		{
			for(auto& rect2 : rects2)
			{
				CollisionRectPair pair(rect1, rect2);
				if(priorityPairs.indexOf(pair)==(size_t)-1)
				{
					pairs.add(pair);
				}
			}
		}
		return pairs;
	}
	
	ArrayList<CollisionRectTagPair> CollisionPair::getReverseCollidedRectPairs() const
	{
		ArrayList<CollisionRectTagPair> reverseCollidedRectPairs;
		reverseCollidedRectPairs.reserve(collidedRectPairs.size());
		for(auto& rectPair : collidedRectPairs)
		{
			reverseCollidedRectPairs.add(CollisionRectTagPair(rectPair.second, rectPair.first));
		}
		return reverseCollidedRectPairs;
	}
	
	ArrayList<CollisionSide> CollisionPair::getOppositeSides() const
	{
		ArrayList<CollisionSide> oppositeSides;
		oppositeSides.reserve(sides.size());
		for(auto& side : sides)
		{
			oppositeSides.add(CollisionSide_getOpposite(side));
		}
		return oppositeSides;
	}
	
	ArrayList<CollisionRectTagPair> CollisionPair::getReverseIgnoredRectPairs() const
	{
		ArrayList<CollisionRectTagPair> reverseIgnoredRectPairs;
		reverseIgnoredRectPairs.reserve(ignoredRectPairs.size());
		for(auto& rectPair : ignoredRectPairs)
		{
			reverseIgnoredRectPairs.add(CollisionRectTagPair(rectPair.second, rectPair.first));
		}
		return reverseIgnoredRectPairs;
	}
	
	bool CollisionPair::isContacting() const
	{
		if(collidedRectPairs.size() > 0 || ignoredRectPairs.size() > 0)
		{
			return true;
		}
		return false;
	}
}
