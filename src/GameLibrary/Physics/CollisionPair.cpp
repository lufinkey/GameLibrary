
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

	bool CollisionPair::shouldIgnoreCollision(const CollisionRect* rect1, const CollisionRect* rect2) const
	{
		auto tag1 = rect1->getTag();
		auto tag2 = rect2->getTag();
		for(auto& tagPair : ignoredRectPairs) {
			if(tagPair.first==tag1 && tagPair.second==tag2) {
				return true;
			}
		}
		return false;
	}
	
	std::list<CollisionRectTagPair> CollisionPair::getContactingRectPairs() const
	{
		auto pairs = collidedRectPairs;
		pairs.insert(pairs.end(), ignoredRectPairs.begin(), ignoredRectPairs.end());
		return pairs;
	}
	
	std::list<CollisionRectTagPair> CollisionPair::getReverseContactingRectPairs() const
	{
		auto contactingRectPairs = getContactingRectPairs();
		auto reverseContactingRectPairs = std::list<CollisionRectTagPair>();
		for(auto& pair : contactingRectPairs) {
			reverseContactingRectPairs.push_back(CollisionRectTagPair(pair.second, pair.first));
		}
		return reverseContactingRectPairs;
	}

	std::list<CollisionRectPair> CollisionPair::getCollisionRectPairs(const ArrayList<const CollisionRect*>& rects1, const ArrayList<const CollisionRect*>& rects2) const
	{
		if(collidable1->isSensor() || collidable2->isSensor()) {
			std::list<CollisionRectPair> pairs;
			for(auto rect1 : rects1) {
				for(auto rect2 : rects2) {
					pairs.push_back(CollisionRectPair(rect1, rect2));
				}
			}
			return pairs;
		}
		else {
			auto priorityRectPairs = getContactingRectPairs();
			size_t pair_count = rects1.size()*rects2.size();
			std::list<CollisionRectPair> pairs;
			for(auto& priorityRect : priorityRectPairs) {
				size_t rectIndex1 = rects1.indexWhere([&priorityRect](auto& rect) -> bool {
					if(rect->getTag()==priorityRect.first) {
						return true;
					}
					return false;
				});
				size_t rectIndex2 = rects2.indexWhere([&priorityRect](auto& rect) -> bool {
					if(rect->getTag()==priorityRect.second) {
						return true;
					}
					return false;
				});
				if(rectIndex1!=-1 && rectIndex2!=-1) {
					pairs.push_back(CollisionRectPair(rects1[rectIndex1], rects2[rectIndex2]));
				}
			}
			auto priorityPairs = pairs;
			for(auto rect1 : rects1) {
				for(auto rect2 : rects2) {
					CollisionRectPair pair(rect1, rect2);
					if(std::find(priorityPairs.begin(), priorityPairs.end(), pair) == priorityPairs.end()) {
						pairs.push_back(pair);
					}
				}
			}
			return pairs;
		}
	}
	
	std::list<CollisionRectTagPair> CollisionPair::getReverseCollidedRectPairs() const {
		std::list<CollisionRectTagPair> reverseCollidedRectPairs;
		for(auto& rectPair : collidedRectPairs) {
			reverseCollidedRectPairs.push_back(CollisionRectTagPair(rectPair.second, rectPair.first));
		}
		return reverseCollidedRectPairs;
	}
	
	std::list<CollisionSide> CollisionPair::getOppositeSides() const {
		std::list<CollisionSide> oppositeSides;
		for(auto& side : sides) {
			oppositeSides.push_back(CollisionSide_getOpposite(side));
		}
		return oppositeSides;
	}
	
	std::list<CollisionRectTagPair> CollisionPair::getReverseIgnoredRectPairs() const {
		std::list<CollisionRectTagPair> reverseIgnoredRectPairs;
		for(auto& rectPair : ignoredRectPairs) {
			reverseIgnoredRectPairs.push_back(CollisionRectTagPair(rectPair.second, rectPair.first));
		}
		return reverseIgnoredRectPairs;
	}
	
	bool CollisionPair::isContacting() const {
		if(collidedRectPairs.size() > 0 || ignoredRectPairs.size() > 0) {
			return true;
		}
		return false;
	}
}
