
#include <GameLibrary/Physics/CollisionManager.hpp>

namespace fgl
{
	CollisionManager::CollisionManager()
	{
		//
	}

	void CollisionManager::addCollidable(Collidable* collidable)
	{
		collidables.add(collidable);
	}

	void CollisionManager::removeCollidable(Collidable* collidable)
	{
		size_t index = collidables.indexOf(collidable);
		if(index!=(size_t)-1)
		{
			collidables.remove(index);
		}
	}
	
	const ArrayList<Collidable*>& CollisionManager::getCollidables() const
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
		// update velocities
		auto frameSpeedMultiplier = appData.getFrameSpeedMultiplier();
		for(auto& collidable : collidables)
		{
			collidable->updateVelocity(frameSpeedMultiplier);
		}
		
		// start collision calculations
		for(auto& collidable : collidables)
		{
			collidable->onBeginCollisionUpdates();
		}

		ArrayList<CollisionPair> pairs = getCollisionPairs();
		ArrayList<CollisionPair> collisions;

		ArrayList<std::function<void()>> onContactCalls;
		ArrayList<std::function<void()>> onContactFinishCalls;

		ArrayList<std::function<void()>> onCollisionCalls;
		ArrayList<std::function<void()>> onCollisionFinishCalls;

		//handle collisions
		#ifdef DOUBLECHECK_COLLISIONS
		//checking all of the collisions twice fixes collision jerking
		for(size_t i=0; i<2; i++)
		#endif
		{
			for(auto& pair : pairs)
			{
				Collidable* collidable1 = pair.collidable1;
				Collidable* collidable2 = pair.collidable2;
				CollisionPair newPair(collidable1, collidable2);
				#ifdef DOUBLECHECK_COLLISIONS
					size_t pairReplaceIndex = -1;
					if(i==1)
					{
						pairReplaceIndex = collisions.indexOf(newPair);
						if(pairReplaceIndex!=-1)
						{
							newPair = collisions[pairReplaceIndex];
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
							//make sure that this collision wasn't previously ignored
							if(pair.shouldIgnoreCollision(rectPair.first, rectPair.second))
							{
								//since the collision was previously ignored, ignore it again
								if(!newPair.ignoredCollisions.contains(rectTagPair))
								{
									newPair.ignoredCollisions.add(rectTagPair);
								}
							}
							else
							{
								//check if we should ignore this collision
								bool ignore = false;
								if(!collidable1->respondsToCollision(collidable2, collisionSide1))
								{
									ignore = true;
								}
								else if(!collidable2->respondsToCollision(collidable1, collisionSide2))
								{
									ignore = true;
								}

								if(ignore)
								{
									//ignore collision
									if(!newPair.ignoredCollisions.contains(rectTagPair))
									{
										newPair.ignoredCollisions.add(rectTagPair);
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
										ArrayList<CollisionPair> collisionsSoFar;
										collisionsSoFar.reserve(previousCollisions.size()+collisions.size());
										collisionsSoFar.addAll(previousCollisions);
										collisionsSoFar.addAll(collisions);
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

											Vector2d moveAmount1;
											Vector2d moveAmount2;
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
								}
							}

							//add the rect pair to the priority rects, so that it will be checked first on the next frame
							if(!newPair.priorityRects.contains(rectTagPair))
							{
								newPair.priorityRects.add(rectTagPair);
							}
						}
					}

					//add new collision pair to previous collisions
					#ifdef DOUBLECHECK_COLLISIONS
					if(pairReplaceIndex==-1)
					{
					#endif
						if(newPair.priorityRects.size() > 0)
						{
							collisions.add(newPair);
						}
					#ifdef DOUBLECHECK_COLLISIONS
					}
					else
					{
						collisions[pairReplaceIndex] = newPair;
					}
					#endif
				}

				#ifdef DOUBLECHECK_COLLISIONS
				if(i==1)
				#endif
				{
					//check if is/was contacting
					if(newPair.isContacting())
					{
						if(pair.isContacting())
						{
							//updated contact
							auto contactEvent1 = ContactEvent(collidable2, CONTACTSTATE_UPDATED);
							auto contactEvent2 = ContactEvent(collidable1, CONTACTSTATE_UPDATED);
							if(collidable1->isStaticCollisionBody())
							{
								onContactCalls.add([=] {
									collidable2->onContactUpdate(contactEvent2);
									collidable1->onContactUpdate(contactEvent1);
								});
							}
							else if(collidable2->isStaticCollisionBody())
							{
								onContactCalls.add([=] {
									collidable1->onContactUpdate(contactEvent1);
									collidable2->onContactUpdate(contactEvent2);
								});
							}
							else
							{
								double randomFirst = Math::random();
								if(randomFirst < 0.5)
								{
									onContactCalls.add([=] {
										collidable1->onContactUpdate(contactEvent1);
										collidable2->onContactUpdate(contactEvent2);
									});
								}
								else
								{
									onContactCalls.add([=] {
										collidable2->onContactUpdate(contactEvent2);
										collidable1->onContactUpdate(contactEvent1);
									});
								}
							}
						}
						else
						{
							//new contact
							auto contactEvent1 = ContactEvent(collidable2, CONTACTSTATE_NEW);
							auto contactEvent2 = ContactEvent(collidable1, CONTACTSTATE_NEW);
							if(collidable1->isStaticCollisionBody())
							{
								onContactCalls.add([=] {
									collidable2->onContact(contactEvent2);
									collidable1->onContact(contactEvent1);
								});
							}
							else if(collidable2->isStaticCollisionBody())
							{
								onContactCalls.add([=] {
									collidable1->onContact(contactEvent1);
									collidable2->onContact(contactEvent2);
								});
							}
							else
							{
								double randomFirst = Math::random();
								if(randomFirst < 0.5)
								{
									onContactCalls.add([=] {
										collidable1->onContact(contactEvent1);
										collidable2->onContact(contactEvent2);
									});
								}
								else
								{
									onContactCalls.add([=] {
										collidable2->onContact(contactEvent2);
										collidable1->onContact(contactEvent1);
									});
								}
							}
						}
					}
					else if(pair.isContacting())
					{
						//finished contact
						auto contactEvent1 = ContactEvent(collidable2, CONTACTSTATE_FINISHED);
						auto contactEvent2 = ContactEvent(collidable1, CONTACTSTATE_FINISHED);
						if(collidable1->isStaticCollisionBody())
						{
							onContactFinishCalls.add([=] {
								collidable2->onContactFinish(contactEvent2);
								collidable1->onContactFinish(contactEvent1);
							});
						}
						else if(collidable2->isStaticCollisionBody())
						{
							onContactFinishCalls.add([=] {
								collidable1->onContactFinish(contactEvent1);
								collidable2->onContactFinish(contactEvent2);
							});
						}
						else
						{
							double randomFirst = Math::random();
							if(randomFirst < 0.5)
							{
								onContactFinishCalls.add([=] {
									collidable1->onContactFinish(contactEvent1);
									collidable2->onContactFinish(contactEvent2);
								});
							}
							else
							{
								onContactFinishCalls.add([=] {
									collidable2->onContactFinish(contactEvent2);
									collidable1->onContactFinish(contactEvent1);
								});
							}
						}
					}

					//check for new/updated collision calls
					for(auto collisionSide : newPair.sides)
					{
						if(!pair.sides.contains(collisionSide))
						{
							//the previous collision pair doesn't have this collision side, so it is a new collision
							auto collisionEvent1 = CollisionEvent(collidable2, collisionSide, COLLISIONSTATE_NEW);
							auto collisionEvent2 = CollisionEvent(collidable1, CollisionSide_getOpposite(collisionSide), COLLISIONSTATE_NEW);
							if(collidable1->isStaticCollisionBody())
							{
								onCollisionCalls.add([=] {
									collidable2->onCollision(collisionEvent2);
									collidable1->onCollision(collisionEvent1);
								});
							}
							else if(collidable2->isStaticCollisionBody())
							{
								onCollisionCalls.add([=] {
									collidable1->onCollision(collisionEvent1);
									collidable2->onCollision(collisionEvent2);
								});
							}
							else
							{
								double randomFirst = Math::random();
								if(randomFirst < 0.5)
								{
									onCollisionCalls.add([=] {
										collidable1->onCollision(collisionEvent1);
										collidable2->onCollision(collisionEvent2);
									});
								}
								else
								{
									onCollisionCalls.add([=] {
										collidable2->onCollision(collisionEvent2);
										collidable1->onCollision(collisionEvent1);
									});
								}
							}
						}
						else
						{
							//the previous collision pair has this collision side, so it's an updated collision
							auto collisionEvent1 = CollisionEvent(collidable2, collisionSide, COLLISIONSTATE_UPDATED);
							auto collisionEvent2 = CollisionEvent(collidable1, CollisionSide_getOpposite(collisionSide), COLLISIONSTATE_UPDATED);
							if(collidable1->isStaticCollisionBody())
							{
								onCollisionCalls.add([=] {
									collidable2->onCollisionUpdate(collisionEvent2);
									collidable1->onCollisionUpdate(collisionEvent1);
								});
							}
							else if(collidable2->isStaticCollisionBody())
							{
								onCollisionCalls.add([=] {
									collidable1->onCollisionUpdate(collisionEvent1);
									collidable2->onCollisionUpdate(collisionEvent2);
								});
							}
							else
							{
								double randomFirst = Math::random();
								if(randomFirst < 0.5)
								{
									onCollisionCalls.add([=] {
										collidable1->onCollisionUpdate(collisionEvent1);
										collidable2->onCollisionUpdate(collisionEvent2);
									});
								}
								else
								{
									onCollisionCalls.add([=] {
										collidable2->onCollisionUpdate(collisionEvent2);
										collidable1->onCollisionUpdate(collisionEvent1);
									});
								}
							}
						}
					}

					//check for finished collision calls
					for(auto prevCollisionSide : pair.sides)
					{
						if(!newPair.sides.contains(prevCollisionSide))
						{
							auto collisionEvent1 = CollisionEvent(collidable2, prevCollisionSide, COLLISIONSTATE_FINISHED);
							auto collisionEvent2 = CollisionEvent(collidable1, CollisionSide_getOpposite(prevCollisionSide), COLLISIONSTATE_FINISHED);
							if(collidable1->isStaticCollisionBody())
							{
								onCollisionFinishCalls.add([=]{
									collidable2->onCollisionFinish(collisionEvent2);
									collidable1->onCollisionFinish(collisionEvent1);
								});
							}
							else if(collidable2->isStaticCollisionBody())
							{
								onCollisionFinishCalls.add([=]{
									collidable1->onCollisionFinish(collisionEvent1);
									collidable2->onCollisionFinish(collisionEvent2);
								});
							}
							else
							{
								double randomFirst = Math::random();
								if(randomFirst < 0.5)
								{
									onCollisionCalls.add([=] {
										collidable1->onCollisionFinish(collisionEvent1);
										collidable2->onCollisionFinish(collisionEvent2);
									});
								}
								else
								{
									onCollisionCalls.add([=] {
										collidable2->onCollisionFinish(collisionEvent2);
										collidable1->onCollisionFinish(collisionEvent1);
									});
								}
							}
						}
					}
				}
			}
		}

		previousCollisions = collisions;

		//call finished collisions
		for(auto& onCollisionFinish : onCollisionFinishCalls)
		{
			onCollisionFinish();
		}

		//call finished contacts
		for(auto& onContactFinish : onContactFinishCalls)
		{
			onContactFinish();
		}

		//call current contacts
		for(auto& onContact : onContactCalls)
		{
			onContact();
		}

		//call current collisions
		for(auto& onCollision : onCollisionCalls)
		{
			onCollision();
		}

		//set the previous positions of the collidables
		for(auto& collidable : collidables)
		{
			auto transformState = collidable->getTransformState();
			auto prevTransformState = collidable->getPreviousTransformState();
			collidable->displacement = (transformState.position - prevTransformState.position);
			collidable->previousTransformState = transformState;
		}

		//tell updated collidables that their collision updates have finished
		for(auto& collidable : collidables)
		{
			collidable->onFinishCollisionUpdates();
		}
	}

	ArrayList<CollisionPair> CollisionManager::getCollisionPairs() const
	{
		ArrayList<CollisionPair> pairs;
		ArrayList<CollisionPair> prevStaticCollisions;
		prevStaticCollisions.reserve(previousCollisions.size());
		ArrayList<CollisionPair> prevNonstaticCollisions;
		prevNonstaticCollisions.reserve(previousCollisions.size());
		for(auto& collisionPair : previousCollisions)
		{
			if(collisionPair.collidable1->isStaticCollisionBody() || collisionPair.collidable2->isStaticCollisionBody())
			{
				prevStaticCollisions.add(collisionPair);
			}
			else
			{
				prevNonstaticCollisions.add(collisionPair);
			}
		}
		
		size_t collidables_size = collidables.size();
		size_t nSquared = collidables_size*collidables_size;
		
		ArrayList<CollisionPair> staticCollisions;
		staticCollisions.reserve(nSquared);
		ArrayList<CollisionPair> nonstaticCollisions;
		nonstaticCollisions.reserve(nSquared);
		for(size_t i=0; i<collidables_size; i++)
		{
			for(size_t j=(i+1); j<collidables_size; j++)
			{
				CollisionPair pair(collidables[i], collidables[j]);
				if(previousCollisions.indexOf(pair)==(size_t)-1)
				{
					if(!(pair.collidable1->isStaticCollisionBody() && pair.collidable2->isStaticCollisionBody()))
					{
						if(pair.collidable1->isStaticCollisionBody() || pair.collidable2->isStaticCollisionBody())
						{
							staticCollisions.add(pair);
						}
						else
						{
							nonstaticCollisions.add(pair);
						}
					}
				}
			}
		}
		pairs.reserve(prevStaticCollisions.size()+staticCollisions.size()+prevNonstaticCollisions.size()+nonstaticCollisions.size());
		pairs.addAll(prevStaticCollisions);
		pairs.addAll(staticCollisions);
		pairs.addAll(prevNonstaticCollisions);
		pairs.addAll(nonstaticCollisions);
		return pairs;
	}
}
