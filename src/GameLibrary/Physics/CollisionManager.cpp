
#include <GameLibrary/Physics/CollisionManager.hpp>

namespace fgl
{
	CollisionManager::CollisionManager()
	{
		//
	}

	void CollisionManager::addCollidable(Collidable* collidable)
	{
		collidables.push_back(collidable);
	}

	void CollisionManager::removeCollidable(Collidable* collidable)
	{
		for(auto it=collidables.begin(); it!=collidables.end(); it++)
		{
			auto cmp = *it;
			if(collidable == cmp)
			{
				collidables.erase(it);
				break;
			}
		}
	}
	
	const std::list<Collidable*>& CollisionManager::getCollidables() const
	{
		return collidables;
	}
	
	CollisionSide CollisionManager::getCollisionSide(const Vector2d& shiftAmount) const
	{
		if(shiftAmount.x < 0)
		{
			return COLLISIONSIDE_LEFT;
		}
		else if(shiftAmount.x > 0)
		{
			return COLLISIONSIDE_RIGHT;
		}
		else if(shiftAmount.y < 0)
		{
			return COLLISIONSIDE_TOP;
		}
		else if(shiftAmount.y > 0)
		{
			return COLLISIONSIDE_BOTTOM;
		}
		throw IllegalArgumentException("shiftAmount", "cannot be 0,0");
	}

#define DOUBLECHECK_COLLISIONS

	void CollisionManager::update(const ApplicationData& appData)
	{
		// start collision calculations
		for(auto& collidable : collidables)
		{
			collidable->onBeginCollisionUpdates();
		}

		std::list<CollisionPair> pairs = getCollisionPairs();
		std::list<CollisionPair> collisions;

		UpdateData updateData;

		//handle collisions
		#ifdef DOUBLECHECK_COLLISIONS
		//checking all of the collisions twice fixes collision jerking
		for(size_t i=0; i<2; i++)
		#endif
		{
			for(auto it=pairs.begin(); it!=pairs.end(); it++)
			{
				auto& pair = *it;
				Collidable* collidable1 = pair.collidable1;
				Collidable* collidable2 = pair.collidable2;
				CollisionPair newPair(collidable1, collidable2);
				#ifdef DOUBLECHECK_COLLISIONS
					auto pairReplaceIter = collisions.end();
					if(i==1)
					{
						for(auto it2=collisions.begin(); it2!=collisions.end(); it2++)
						{
							auto& cmpPair = *it2;
							if(newPair == cmpPair)
							{
								pairReplaceIter = it2;
								newPair = cmpPair;
								break;
							}
						}
					}
				#endif

				if(!(collidable1->isStaticCollisionBody() && collidable2->isStaticCollisionBody()))
				{
					ArrayList<CollisionRect*> rects1 = collidable1->getCollisionRects();
					ArrayList<CollisionRect*> rects2 = collidable2->getCollisionRects();

					ArrayList<CollisionRectPair> rectPairs = pair.getCollisionRectPairs(rects1, rects2);
					//check each CollisionRect for a collision
					for(auto& rectPair : rectPairs)
					{
						Vector2d shiftAmount = CollisionRect::getCollisionOffset(rectPair.first, rectPair.second);
						if(!(shiftAmount.x==0 && shiftAmount.y==0))
						{
							CollisionSide collisionSide1 = getCollisionSide(shiftAmount);
							CollisionSide collisionSide2 = CollisionSide_getOpposite(collisionSide1);
							CollisionRectTagPair rectTagPair = CollisionRectTagPair(rectPair.first->getTag(), rectPair.second->getTag());
							//check if we should ignore this collision
							if(pair.shouldIgnoreCollision(rectPair.first, rectPair.second)
								|| !respondsToCollision(collidable1, collidable2, rectPair, collisionSide1)
								|| !collidable1->respondsToCollision(collidable2, collisionSide1, rectPair)
								|| !collidable2->respondsToCollision(collidable1, collisionSide2, CollisionRectPair(rectPair.second, rectPair.first)))
							{
								//ignore collision
								if(!newPair.ignoredRectPairs.contains(rectTagPair))
								{
									newPair.ignoredRectPairs.add(rectTagPair);
								}
							}
							else
							{
								//decide how to shift the collidables
								if(collidable1->isStaticCollisionBody())
								{
									collidable2->shift(shiftAmount);
									for(auto& rect : rects2)
									{
										rect->shift(shiftAmount);
									}
								}
								else if(collidable2->isStaticCollisionBody())
								{
									collidable1->shift(-shiftAmount);
									for(auto& rect : rects1)
									{
										rect->shift(-shiftAmount);
									}
								}
								else
								{
									//find out if rect1 has collided with a static collision body
									std::list<CollisionPair> collisionsSoFar;
									collisionsSoFar.insert(collisionsSoFar.end(), previousCollisions.begin(), previousCollisions.end());
									collisionsSoFar.insert(collisionsSoFar.end(), collisions.begin(), collisions.end());
									bool staticOpposite1 = false;
									for(auto& collision : collisionsSoFar)
									{
										if(collision.collidable1==collidable1 && collision.collidable2->isStaticCollisionBody() && collision.sides.contains(collisionSide2))
										{
											staticOpposite1 = true;
											break;
										}
										else if(collision.collidable2==collidable1 && collision.collidable1->isStaticCollisionBody() && collision.sides.contains(collisionSide1))
										{
											staticOpposite1 = true;
											break;
										}
									}
									bool staticOpposite2 = false;
									for(auto& collision : collisionsSoFar)
									{
										if(collision.collidable1==collidable2 && collision.collidable2->isStaticCollisionBody() && collision.sides.contains(collisionSide1))
										{
											staticOpposite2 = true;
											break;
										}
										else if(collision.collidable2==collidable2 && collision.collidable1->isStaticCollisionBody() && collision.sides.contains(collisionSide2))
										{
											staticOpposite2 = true;
											break;
										}
									}

									double portion1 = 0;
									if((staticOpposite1 && staticOpposite2) || (!staticOpposite1 && !staticOpposite2))
									{
										//TODO make a BETTER case here for two non-static bodies colliding
										double mass1 = collidable1->getMass();
										double mass2 = collidable2->getMass();
										auto rect1 = rectPair.first->getRect();
										auto prevRect1 = rectPair.first->getPreviousRect();
										auto rect2 = rectPair.second->getRect();
										auto prevRect2 = rectPair.second->getPreviousRect();

										double velocity1 = 0;
										double velocity2 = 0;
										switch(collisionSide1)
										{
											case COLLISIONSIDE_LEFT:
											velocity1 = rect1.x - prevRect1.x;
											velocity2 = (rect2.x+rect2.width) - (prevRect2.x+prevRect2.width);
											break;

											case COLLISIONSIDE_RIGHT:
											velocity1 = (rect1.x+rect1.width) - (prevRect1.x+prevRect1.width);
											velocity2 = rect2.x - prevRect2.x;
											break;

											case COLLISIONSIDE_TOP:
											velocity1 = rect1.y - prevRect1.y;
											velocity2 = (rect2.y+rect2.height) - (prevRect2.y+prevRect2.height);
											break;

											case COLLISIONSIDE_BOTTOM:
											velocity1 = (rect1.y+rect1.height) - (prevRect1.y+prevRect1.height);
											velocity2 = rect2.y - prevRect2.y;
											break;
										}

										double force1 = velocity2*mass2;
										double force2 = velocity1*mass1;
										double negativeForce = 0;
										double positiveForce = 0;
										if(force1 < 0)
										{
											negativeForce += force1;
										}
										else
										{
											positiveForce += force1;
										}
										if(force2 < 0)
										{
											negativeForce += force2;
										}
										else
										{
											positiveForce += force2;
										}
										double netForce = force1 + force2;

										if(netForce < 0)
										{
											double portion = 0;
											if(negativeForce!=0)
											{
												portion = netForce/negativeForce;
											}
											switch(collisionSide1)
											{
												case COLLISIONSIDE_LEFT:
												case COLLISIONSIDE_TOP:
												portion1 = 1.0-portion;
												break;

												case COLLISIONSIDE_RIGHT:
												case COLLISIONSIDE_BOTTOM:
												portion1 = portion;
												break;
											}
										}
										else if(netForce!=0)
										{
											double portion = 0;
											if(positiveForce!=0)
											{
												portion = netForce/positiveForce;
											}
											switch(collisionSide1)
											{
												case COLLISIONSIDE_LEFT:
												case COLLISIONSIDE_TOP:
												portion1 = portion;
												break;

												case COLLISIONSIDE_RIGHT:
												case COLLISIONSIDE_BOTTOM:
												portion1 = 1.0-portion;
												break;
											}
										}
									}
									else if(staticOpposite1)
									{
										portion1 = 0;
									}
									else if(staticOpposite2)
									{
										portion1 = 1.0;
									}

									auto moveAmount1 = -shiftAmount*portion1;
									auto moveAmount2 = shiftAmount+moveAmount1;

									if(moveAmount1.x!=0 || moveAmount1.y!=0)
									{
										collidable1->shift(moveAmount1);
										for(auto& rect : rects1)
										{
											rect->shift(moveAmount1);
										}
									}
									if(moveAmount2.x!=0 || moveAmount2.y!=0)
									{
										collidable2->shift(moveAmount2);
										for(auto& rect : rects2)
										{
											rect->shift(moveAmount2);
										}
									}
								}

								//add collision side to previous collision sides if not already added
								if(!newPair.sides.contains(collisionSide1))
								{
									newPair.sides.add(collisionSide1);
								}
								//add the rect pair to the list of collided rect pairs
								if(!newPair.collidedRectPairs.contains(rectTagPair))
								{
									newPair.collidedRectPairs.add(rectTagPair);
								}
							}
						}
					}

					//add new collision pair to previous collisions
					#ifdef DOUBLECHECK_COLLISIONS
					if(pairReplaceIter == collisions.end())
					{
					#endif
						if(newPair.collidedRectPairs.size() > 0 || newPair.ignoredRectPairs.size() > 0)
						{
							collisions.push_back(newPair);
						}
					#ifdef DOUBLECHECK_COLLISIONS
					}
					else
					{
						*pairReplaceIter = newPair;
					}
					#endif
				}

				#ifdef DOUBLECHECK_COLLISIONS
				if(i==1)
				#endif
				{
					performFinalCollisionUpdates(newPair, pair, updateData);
					//check if is/was contacting
					if(newPair.isContacting())
					{
						if(pair.isContacting())
						{
							//updated contact
							updateData.onContactCalls.push_back([=] {
								dispatchContactEvents(CONTACTSTATE_UPDATED, newPair, pair);
							});
						}
						else
						{
							//new contact
							updateData.onContactCalls.push_back([=] {
								dispatchContactEvents(CONTACTSTATE_NEW, newPair, pair);
							});
						}
					}
					else if(pair.isContacting())
					{
						//finished contact
						updateData.onContactFinishCalls.push_back([=] {
							dispatchContactEvents(CONTACTSTATE_FINISHED, newPair, pair);
						});
					}

					//check for new/updated collision calls
					for(auto collisionSide : newPair.sides)
					{
						if(!pair.sides.contains(collisionSide))
						{
							//the previous collision pair doesn't have this collision side, so it is a new collision
							updateData.onCollisionCalls.push_back([=] {
								dispatchCollisionEvents(COLLISIONSTATE_NEW, collisionSide, newPair, pair);
							});
						}
						else
						{
							//the previous collision pair has this collision side, so it's an updated collision
							updateData.onCollisionCalls.push_back([=] {
								dispatchCollisionEvents(COLLISIONSTATE_UPDATED, collisionSide, newPair, pair);
							});
						}
					}

					//check for finished collision calls
					for(auto prevCollisionSide : pair.sides)
					{
						if(!newPair.sides.contains(prevCollisionSide))
						{
							updateData.onCollisionFinishCalls.push_back([=] {
								dispatchCollisionEvents(COLLISIONSTATE_FINISHED, prevCollisionSide, newPair, pair);
							});
						}
					}
				}
			}
		}
		
		onWillFinishCollisionUpdates(updateData);
		
		previousCollisions = collisions;

		//call finished collisions
		for(auto& onCollisionFinish : updateData.onCollisionFinishCalls)
		{
			onCollisionFinish();
		}

		//call finished contacts
		for(auto& onContactFinish : updateData.onContactFinishCalls)
		{
			onContactFinish();
		}

		//call current contacts
		for(auto& onContact : updateData.onContactCalls)
		{
			onContact();
		}

		//call current collisions
		for(auto& onCollision : updateData.onCollisionCalls)
		{
			onCollision();
		}

		//set the previous positions of the collidables
		for(auto& collidable : collidables)
		{
			collidable->updateTransformState();
		}

		//tell updated collidables that their collision updates have finished
		for(auto& collidable : collidables)
		{
			collidable->onFinishCollisionUpdates();
		}
		
		onFinishCollisionUpdates();
	}

	std::list<CollisionPair> CollisionManager::getCollisionPairs() const
	{
		std::list<CollisionPair> pairs;
		
		std::list<CollisionPair> prevStaticCollisions;
		std::list<CollisionPair> prevNonstaticCollisions;
		for(auto& collisionPair : previousCollisions)
		{
			if(collisionPair.collidable1->isStaticCollisionBody() || collisionPair.collidable2->isStaticCollisionBody())
			{
				prevStaticCollisions.push_back(collisionPair);
			}
			else
			{
				prevNonstaticCollisions.push_back(collisionPair);
			}
		}
		
		std::list<CollisionPair> staticCollisions;
		std::list<CollisionPair> nonstaticCollisions;
		for(auto it=collidables.begin(); it!=collidables.end(); it++)
		{
			for(auto it2=std::next(it, 1); it2!=collidables.end(); it2++)
			{
				auto collidable1 = *it;
				auto collidable2 = *it2;
				CollisionPair pair(collidable1, collidable2);
				bool prevCollisionExists = false;
				for(auto& cmpPair : previousCollisions)
				{
					if(pair == cmpPair)
					{
						prevCollisionExists = true;
						break;
					}
				}
				if(!prevCollisionExists)
				{
					if(!(pair.collidable1->isStaticCollisionBody() && pair.collidable2->isStaticCollisionBody()))
					{
						if(pair.collidable1->isStaticCollisionBody() || pair.collidable2->isStaticCollisionBody())
						{
							staticCollisions.push_back(pair);
						}
						else
						{
							nonstaticCollisions.push_back(pair);
						}
					}
				}
			}
		}
		pairs.splice(pairs.end(), prevStaticCollisions);
		pairs.splice(pairs.end(), staticCollisions);
		pairs.splice(pairs.end(), prevNonstaticCollisions);
		pairs.splice(pairs.end(), nonstaticCollisions);
		return pairs;
	}
	
	bool CollisionManager::respondsToCollision(Collidable* collidable1, Collidable* collidable2, CollisionRectPair rectPair, CollisionSide side) const
	{
		return true;
	}
	
	void CollisionManager::dispatchContactEvents(ContactState state, const CollisionPair& pair, const CollisionPair& prevPair)
	{
		auto collidable1 = pair.collidable1;
		auto collidable2 = pair.collidable2;
		auto contactEvent1 = ContactEvent(collidable1, collidable2, state, pair.getContactingRectPairs(), prevPair.getContactingRectPairs(), pair.ignoredRectPairs, pair.sides);
		auto contactEvent2 = ContactEvent(collidable2, collidable1, state, pair.getReverseContactingRectPairs(), prevPair.getReverseContactingRectPairs(), pair.getReverseIgnoredRectPairs(), pair.getOppositeSides());
		
		if(collidable1->isStaticCollisionBody())
		{
			switch(state)
			{
				case CONTACTSTATE_NEW:
					collidable2->onContact(contactEvent2);
					collidable1->onContact(contactEvent1);
					break;
					
				case CONTACTSTATE_UPDATED:
					collidable2->onContactUpdate(contactEvent2);
					collidable1->onContactUpdate(contactEvent1);
					break;
					
				case CONTACTSTATE_FINISHED:
					collidable2->onContactFinish(contactEvent2);
					collidable1->onContactFinish(contactEvent1);
					break;
			}
		}
		else
		{
			switch(state)
			{
				case CONTACTSTATE_NEW:
					collidable1->onContact(contactEvent1);
					collidable2->onContact(contactEvent2);
					break;
					
				case CONTACTSTATE_UPDATED:
					collidable1->onContactUpdate(contactEvent1);
					collidable2->onContactUpdate(contactEvent2);
					break;
					
				case CONTACTSTATE_FINISHED:
					collidable1->onContactFinish(contactEvent1);
					collidable2->onContactFinish(contactEvent2);
					break;
			}
		}
	}
	
	void CollisionManager::dispatchCollisionEvents(CollisionState state, CollisionSide side, const CollisionPair& pair, const CollisionPair& prevPair)
	{
		auto collidable1 = pair.collidable1;
		auto collidable2 = pair.collidable2;
		auto collisionEvent1 = CollisionEvent(collidable1, collidable2, side, state, pair.getContactingRectPairs(), prevPair.getContactingRectPairs());
		auto collisionEvent2 = CollisionEvent(collidable2, collidable1, CollisionSide_getOpposite(side), state, pair.getReverseContactingRectPairs(), prevPair.getReverseContactingRectPairs());
		if(collidable1->isStaticCollisionBody())
		{
			switch(state)
			{
				case COLLISIONSTATE_NEW:
					collidable2->onCollision(collisionEvent2);
					collidable1->onCollision(collisionEvent1);
					break;
					
				case COLLISIONSTATE_UPDATED:
					collidable2->onCollisionUpdate(collisionEvent2);
					collidable1->onCollisionUpdate(collisionEvent1);
					break;
					
				case COLLISIONSTATE_FINISHED:
					collidable2->onCollisionFinish(collisionEvent2);
					collidable1->onCollisionFinish(collisionEvent1);
					break;
			}
		}
		else
		{
			switch(state)
			{
				case COLLISIONSTATE_NEW:
					collidable1->onCollision(collisionEvent1);
					collidable2->onCollision(collisionEvent2);
					break;
					
				case COLLISIONSTATE_UPDATED:
					collidable1->onCollisionUpdate(collisionEvent1);
					collidable2->onCollisionUpdate(collisionEvent2);
					break;
					
				case COLLISIONSTATE_FINISHED:
					collidable1->onCollisionFinish(collisionEvent1);
					collidable2->onCollisionFinish(collisionEvent2);
					break;
			}
		}
	}
	
	void CollisionManager::performFinalCollisionUpdates(const CollisionPair& pair, const CollisionPair& prevPair, UpdateData& updateData)
	{
		//
	}
	
	void CollisionManager::onWillFinishCollisionUpdates(UpdateData& updateData)
	{
		//
	}
	
	void CollisionManager::onFinishCollisionUpdates()
	{
		//
	}
}
