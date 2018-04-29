
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
		for(auto& tagPair : ignoredCollisions)
		{
			if(tagPair.first==rect1->getTag() && tagPair.second==rect2->getTag())
			{
				return true;
			}
		}
		return false;
	}

	ArrayList<CollisionRectPair> CollisionPair::getCollisionRectPairs(const ArrayList<CollisionRect*>& rects1, const ArrayList<CollisionRect*>& rects2) const
	{
		size_t pair_count = rects1.size()*rects2.size();
		ArrayList<CollisionRectPair> pairs;
		pairs.reserve(pair_count);
		for(auto& priorityRect : priorityRects)
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
		ArrayList<CollisionRectPair> priorityPairs = pairs;
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
	
	ArrayList<CollisionRectTagPair> CollisionPair::getReversePriorityRects() const
	{
		ArrayList<CollisionRectTagPair> reversePriorityRects;
		reversePriorityRects.reserve(priorityRects.size());
		for(auto& rectPair : priorityRects)
		{
			reversePriorityRects.add(std::pair<fgl::String,fgl::String>(rectPair.second, rectPair.first));
		}
		return reversePriorityRects;
	}
	
	bool CollisionPair::isContacting() const
	{
		if(sides.size() > 0 || ignoredCollisions.size() > 0)
		{
			return true;
		}
		return false;
	}
}
