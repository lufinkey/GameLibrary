
#include <GameLibrary/Physics/CollisionManager.hpp>
#include <GameLibrary/Utilities/Time/TimeInterval.hpp>

namespace fgl
{
	//#define PRINT_COLLISION_TIME
	
	CollisionManager::CollisionManager() {
		//
	}

	void CollisionManager::addCollidable(Collidable* collidable) {
		for(auto cmpCollidable : collidables) {
			if(cmpCollidable == collidable) {
				throw IllegalArgumentException("collidable", "already added to CollisionManager");
			}
		}
		collidable->collided.clear();
		collidables.push_back(collidable);
		auto transformState = collidable->getTransformState();
		collidable->previousTransformState = transformState;
	}

	void CollisionManager::removeCollidable(Collidable* collidable) {
		for(auto it=collidables.begin(); it!=collidables.end(); it++) {
			auto cmp = *it;
			if(collidable == cmp) {
				collidables.erase(it);
				collidable->collided.clear();
				collidable->newCollided.clear();
				break;
			}
		}
		
		for(auto it=previousCollisions.begin(); it!=previousCollisions.end(); it++) {
			if(it->collidable1 == collidable || it->collidable2 == collidable) {
				removedCollisions.push_back(*it);
				it = std::prev(previousCollisions.erase(it));
			}
		}
	}
	
	void CollisionManager::addEventListener(CollisionManagerEventListener* eventListener) {
		listeners.push_back(eventListener);
	}
	
	void CollisionManager::removeEventListener(CollisionManagerEventListener* eventListener) {
		auto listenerIt = std::find(listeners.begin(), listeners.end(), eventListener);
		if(listenerIt != listeners.end()) {
			listeners.erase(listenerIt);
		}
	}
	
	const std::list<Collidable*>& CollisionManager::getCollidables() const {
		return collidables;
	}
	
	const std::list<CollisionPair>& CollisionManager::getRemovedCollisions() const {
		return removedCollisions;
	}
	
	CollisionSide CollisionManager::getCollisionSide(const Vector2d& shiftAmount) const {
		if(shiftAmount.x < 0) {
			return CollisionSide::LEFT;
		}
		else if(shiftAmount.x > 0) {
			return CollisionSide::RIGHT;
		}
		else if(shiftAmount.y < 0) {
			return CollisionSide::TOP;
		}
		else if(shiftAmount.y > 0) {
			return CollisionSide::BOTTOM;
		}
		throw IllegalArgumentException("shiftAmount", "cannot be 0,0");
	}
	
	
	
	

#define DOUBLECHECK_COLLISIONS

	void CollisionManager::update(const ApplicationData& appData) {
		#ifdef PRINT_COLLISION_TIME
			auto interval = TimeInterval();
			interval.start();
		#endif
		
		onWillBeginCollisionUpdates(appData);
		
		// call "begin" listener events
		for(auto listener : listeners) {
			listener->onBeginCollisionUpdates(this, appData);
		}
		
		// start collision calculations
		for(auto collidable : collidables) {
			collidable->onBeginCollisionUpdates(appData);
		}
		
		onBeginCollisionUpdates(appData);
		
		// update awareness rects
		for(auto collidable : collidables) {
			auto rect = collidable->getBoundingRect();
			double expandSize = 10;
			collidable->awarenessRect = RectangleD(rect.x-expandSize, rect.y-expandSize, rect.width+(expandSize*2.0), rect.height+(expandSize*2.0));
		}

		std::list<CollisionPair> pairs = getCollisionPairs();
		std::list<CollisionPair> collisions;
		//printf("evaluating %i collision pairs\n", (int)pairs.size());
		
		UpdateData updateData;
		auto appDataPtr = &appData;
		
		//handle collisions
		#ifdef DOUBLECHECK_COLLISIONS
		//checking all of the collisions twice fixes collision jerking
		for(size_t i=0; i<2; i++) {
		#endif
			for(auto it=pairs.begin(); it!=pairs.end(); it++) {
				auto& pair = *it;
				Collidable* collidable1 = pair.collidable1;
				Collidable* collidable2 = pair.collidable2;
				CollisionPair newPair(collidable1, collidable2);
				#ifdef DOUBLECHECK_COLLISIONS
					auto pairReplaceIter = collisions.end();
					if(i!=0) {
						for(auto it2=collisions.begin(); it2!=collisions.end(); it2++) {
							auto& cmpPair = *it2;
							if(newPair == cmpPair) {
								pairReplaceIter = it2;
								newPair = cmpPair;
								break;
							}
						}
					}
				#endif
				
				if(!collidable1->isAwake() && !collidable2->isAwake()) {
					newPair = pair;
					
					auto& collided1 = collidable1->newCollided;
					for(auto side : newPair.sides) {
						auto collidedListIt1 = collided1.find(side);
						if(collidedListIt1 == collided1.end()) {
							collided1[side] = { collidable2 };
						}
						else {
							auto& collidedList1 = collidedListIt1->second;
							auto collidedIt = std::find(collidedList1.begin(), collidedList1.end(), collidable2);
							if(collidedIt == collidedList1.end()) {
								collidedList1.push_back(collidable2);
							}
						}
					}
					
					auto& collided2 = collidable2->newCollided;
					for(auto side : newPair.getOppositeSides()) {
						auto collidedListIt2 = collided2.find(side);
						if(collidedListIt2 == collided2.end()) {
							collided2[side] = { collidable1 };
						}
						else {
							auto& collidedList2 = collidedListIt2->second;
							auto collidedIt = std::find(collidedList2.begin(), collidedList2.end(), collidable1);
							if(collidedIt == collidedList2.end()) {
								collidedList2.push_back(collidable1);
							}
						}
					}
				}
				else if(!(collidable1->isStaticCollisionBody() && collidable2->isStaticCollisionBody())) {
					ArrayList<const CollisionRect*> rects1 = collidable1->getCollisionRects();
					ArrayList<const CollisionRect*> rects2 = collidable2->getCollisionRects();

					ArrayList<CollisionRectPair> rectPairs = pair.getCollisionRectPairs(rects1, rects2);
					if(collidable1->isSensor() || collidable2->isSensor()) {
						// check each CollisionRect for contact
						for(auto& rectPair : rectPairs) {
							if(CollisionRect::checkCollision(collidable1, rectPair.first, collidable2, rectPair.second)) {
								auto rectTagPair = CollisionRectTagPair(rectPair.first->getTag(), rectPair.second->getTag());
								if(std::find(newPair.ignoredRectPairs.begin(), newPair.ignoredRectPairs.end(), rectTagPair) == newPair.ignoredRectPairs.end()) {
									newPair.ignoredRectPairs.push_back(rectTagPair);
								}
							}
						}
					}
					else {
						//check each CollisionRect for a collision
						for(auto& rectPair : rectPairs) {
							Vector2d shiftAmount = CollisionRect::getCollisionOffset(collidable1, rectPair.first, collidable2, rectPair.second);
							if(!(shiftAmount.x==0 && shiftAmount.y==0)) {
								CollisionSide collisionSide1 = getCollisionSide(shiftAmount);
								CollisionSide collisionSide2 = CollisionSide_getOpposite(collisionSide1);
								auto rectTagPair = CollisionRectTagPair(rectPair.first->getTag(), rectPair.second->getTag());
								//check if we should ignore this collision
								if(pair.shouldIgnoreCollision(rectPair.first, rectPair.second)
									|| !respondsToCollision(appData, collidable1, collidable2, rectPair, collisionSide1)
									|| !collidable1->respondsToCollision(collidable2, collisionSide1, rectPair)
									|| !collidable2->respondsToCollision(collidable1, collisionSide2, CollisionRectPair(rectPair.second, rectPair.first))) {
									//ignore collision
									if(std::find(newPair.ignoredRectPairs.begin(), newPair.ignoredRectPairs.end(), rectTagPair) == newPair.ignoredRectPairs.end()) {
										newPair.ignoredRectPairs.push_back(rectTagPair);
									}
								}
								else {
									//decide how to shift the collidables
									if(collidable1->isStaticCollisionBody()) {
										collidable2->shift(shiftAmount);
										collidable2->shiftCollisionsOnSide(collisionSide1, shiftAmount);
									}
									else if(collidable2->isStaticCollisionBody()) {
										collidable1->shift(-shiftAmount);
										collidable1->shiftCollisionsOnSide(collisionSide2, -shiftAmount);
									}
									else {
										//find out if rect1 has collided with a static collision body
										bool staticOpposite1 = collidable1->hasStaticCollisionOnSide(collisionSide2);
										bool staticOpposite2 = collidable2->hasStaticCollisionOnSide(collisionSide1);

										double portion1 = 0.0;
										
										if((staticOpposite1 && staticOpposite2) || (!staticOpposite1 && !staticOpposite2)) {
											//TODO make a BETTER case here for two non-static bodies colliding
											auto transformState1 = collidable1->getTransformState();
											auto prevTransformState1 = collidable1->getPreviousTransformState();
											auto transformState2 = collidable2->getTransformState();
											auto prevTransformState2 = collidable2->getPreviousTransformState();
											auto collided1 = collidable1->getCollidedOnSide(collisionSide2);
											auto collided2 = collidable2->getCollidedOnSide(collisionSide1);
											double mass1 = collidable1->getMass() + collidable1->getCollidedMassOnSide(collisionSide2);
											double mass2 = collidable2->getMass() + collidable2->getCollidedMassOnSide(collisionSide1);
											auto rect1 = rectPair.first->getRect();
											auto prevRect1 = rectPair.first->getPreviousRect();
											auto rect2 = rectPair.second->getRect();
											auto prevRect2 = rectPair.second->getPreviousRect();

											double velocity1 = 0;
											double velocity2 = 0;
											switch(collisionSide1) {
												case CollisionSide::LEFT:
												velocity1 = 
													(transformState1.position.x - prevTransformState1.position.x) +
													(rect1.getLeft() - prevRect1.getLeft());
												velocity2 =
													(transformState2.position.x - prevTransformState2.position.x) +
													(rect2.getRight() - prevRect2.getRight());
												break;

												case CollisionSide::RIGHT:
												velocity1 =
													(transformState1.position.x - prevTransformState1.position.x) +
													(rect1.getRight() - prevRect1.getRight());
												velocity2 = 
													(transformState2.position.x - prevTransformState2.position.x) +
													(rect2.getLeft() - prevRect2.getLeft());
												break;

												case CollisionSide::TOP:
												velocity1 =
													(transformState1.position.y - prevTransformState1.position.y) +
													(rect1.getTop() - prevRect1.getTop());
												velocity2 =
													(transformState2.position.y - prevTransformState2.position.y) +
													(rect2.getBottom() - prevRect2.getBottom());
												break;

												case CollisionSide::BOTTOM:
												velocity1 =
													(transformState1.position.y - prevTransformState1.position.y) +
													(rect1.getBottom() - prevRect1.getBottom());
												velocity2 =
													(transformState2.position.y - prevTransformState2.position.y) +
													(rect2.getTop() - prevRect2.getTop());
												break;
											}

											double force1 = velocity1*mass1;
											double force2 = velocity2*mass2;
											double totalMass = mass1 + mass2;
											if(totalMass == 0) {
												portion1 = 1.0;
											}
											else {
												double finalVelocity = (force1 + force2) / totalMass;
												if(finalVelocity == 0) {
													portion1 = 1.0;
												}
												else {
													portion1 = Math::abs(finalVelocity / (Math::abs(velocity1) + Math::abs(velocity2)));
												}
											}
										}
										else if(staticOpposite1) {
											portion1 = 0.0;
										}
										else if(staticOpposite2) {
											portion1 = 1.0;
										}

										auto moveAmount1 = -shiftAmount * portion1;
										auto moveAmount2 = shiftAmount + moveAmount1;

										if(moveAmount1.x!=0 || moveAmount1.y!=0) {
											collidable1->shift(moveAmount1);
											collidable1->shiftCollisionsOnSide(collisionSide2, moveAmount1);
										}
										if(moveAmount2.x!=0 || moveAmount2.y!=0) {
											collidable2->shift(moveAmount2);
											collidable2->shiftCollisionsOnSide(collisionSide1, moveAmount2);
										}
									}
									
									// add collidables to collided lists
									auto& collided1 = collidable1->newCollided;
									auto collidedListIt1 = collided1.find(collisionSide1);
									if(collidedListIt1 == collided1.end()) {
										collided1[collisionSide1] = { collidable2 };
									}
									else {
										auto& collidedList1 = collidedListIt1->second;
										auto collidedIt = std::find(collidedList1.begin(), collidedList1.end(), collidable2);
										if(collidedIt == collidedList1.end()) {
											collidedList1.push_back(collidable2);
										}
									}
									
									auto& collided2 = collidable2->newCollided;
									auto collidedListIt2 = collided2.find(collisionSide2);
									if(collidedListIt2 == collided2.end()) {
										collided2[collisionSide2] = { collidable1 };
									}
									else {
										auto& collidedList2 = collidedListIt2->second;
										auto collidedIt = std::find(collidedList2.begin(), collidedList2.end(), collidable1);
										if(collidedIt == collidedList2.end()) {
											collidedList2.push_back(collidable1);
										}
									}

									//add collision side to previous collision sides if not already added
									if(std::find(newPair.sides.begin(), newPair.sides.end(), collisionSide1) == newPair.sides.end()) {
										newPair.sides.push_back(collisionSide1);
									}
									//add the rect pair to the list of collided rect pairs
									if(std::find(newPair.collidedRectPairs.begin(), newPair.collidedRectPairs.end(), rectTagPair) == newPair.collidedRectPairs.end()) {
										newPair.collidedRectPairs.push_back(rectTagPair);
									}
								}
							}
						}
					}
				}
				
				//add new collision pair to previous collisions
				#ifdef DOUBLECHECK_COLLISIONS
				if(pairReplaceIter == collisions.end()) {
				#endif
					if(newPair.collidedRectPairs.size() > 0 || newPair.ignoredRectPairs.size() > 0) {
						collisions.push_back(newPair);
					}
				#ifdef DOUBLECHECK_COLLISIONS
				}
				else {
					*pairReplaceIter = newPair;
				}
				#endif

				#ifdef DOUBLECHECK_COLLISIONS
				if(i==1)
				#endif
				{
					performFinalCollisionUpdates(appData, newPair, pair, updateData);
					
					// queue contact events if necessary
					if(newPair.collidable1->contactEventListeners.size() > 0 || newPair.collidable2->contactEventListeners.size() > 0) {
						//check if is/was contacting
						if(newPair.isContacting()) {
							if(pair.isContacting()) {
								//updated contact
								updateData.onContactCalls.push_back([=] {
									dispatchContactEvents(*appDataPtr, CONTACTSTATE_UPDATED, newPair, pair);
								});
							}
							else {
								//new contact
								updateData.onContactCalls.push_back([=] {
									dispatchContactEvents(*appDataPtr, CONTACTSTATE_NEW, newPair, pair);
								});
							}
						}
						else if(pair.isContacting()) {
							//finished contact
							updateData.onContactFinishCalls.push_back([=] {
								dispatchContactEvents(*appDataPtr, CONTACTSTATE_FINISHED, newPair, pair);
							});
						}
					}

					// queue collision events if necessary
					if(newPair.collidable1->collisionEventListeners.size() > 0 || newPair.collidable2->collisionEventListeners.size() > 0) {
						//check for new/updated collision calls
						for(auto collisionSide : newPair.sides) {
							if(std::find(pair.sides.begin(), pair.sides.end(), collisionSide) == pair.sides.end()) {
								//the previous collision pair doesn't have this collision side, so it is a new collision
								updateData.onCollisionCalls.push_back([=] {
									dispatchCollisionEvents(*appDataPtr, COLLISIONSTATE_NEW, collisionSide, newPair, pair);
								});
							}
							else {
								//the previous collision pair has this collision side, so it's an updated collision
								updateData.onCollisionCalls.push_back([=] {
									dispatchCollisionEvents(*appDataPtr, COLLISIONSTATE_UPDATED, collisionSide, newPair, pair);
								});
							}
						}

						//check for finished collision calls
						for(auto prevCollisionSide : pair.sides) {
							if(std::find(newPair.sides.begin(), newPair.sides.end(), prevCollisionSide) == newPair.sides.end()) {
								updateData.onCollisionFinishCalls.push_back([=] {
									dispatchCollisionEvents(*appDataPtr, COLLISIONSTATE_FINISHED, prevCollisionSide, newPair, pair);
								});
							}
						}
					}
				}
			}
		#ifdef DOUBLECHECK_COLLISIONS
		}
		#endif
		
		// transfer collided lists
		for(auto& collidable : collidables) {
			collidable->collided.swap(collidable->newCollided);
			collidable->newCollided.clear();
		}
		
		onWillFinishCollisionUpdates(appData, updateData);
		
		// finish removed collisions
		for(auto& pair : removedCollisions) {
			if(pair.collidable1->collisionEventListeners.size() > 0 || pair.collidable2->collisionEventListeners.size() > 0) {
				for(auto side : pair.sides) {
					updateData.onCollisionFinishCalls.push_back([=]() {
						dispatchCollisionEvents(appData, COLLISIONSTATE_FINISHED, side, CollisionPair(pair.collidable1, pair.collidable2), pair);
					});
				}
			}
			if(pair.collidable1->contactEventListeners.size() > 0 || pair.collidable2->contactEventListeners.size() > 0) {
				if(pair.isContacting()) {
					updateData.onContactFinishCalls.push_back([=]() {
						dispatchContactEvents(appData, CONTACTSTATE_FINISHED, CollisionPair(pair.collidable1, pair.collidable2), pair);
					});
				}
			}
		}
		removedCollisions.clear();
		
		// update collision list
		previousCollisions.swap(collisions);
		collisions.clear();

		//call finished collisions
		for(auto& onCollisionFinish : updateData.onCollisionFinishCalls) {
			onCollisionFinish();
		}

		//call finished contacts
		for(auto& onContactFinish : updateData.onContactFinishCalls) {
			onContactFinish();
		}

		//call current contacts
		for(auto& onContact : updateData.onContactCalls) {
			onContact();
		}

		//call current collisions
		for(auto& onCollision : updateData.onCollisionCalls) {
			onCollision();
		}

		//set the previous positions of the collidables
		for(auto& collidable : collidables) {
			collidable->updateTransformState();
		}

		//tell updated collidables that their collision updates have finished
		for(auto& collidable : collidables) {
			collidable->onFinishCollisionUpdates(appData);
		}
		
		onFinishCollisionUpdates(appData);
		
		for(auto listener : listeners) {
			listener->onFinishCollisionUpdates(this, appData);
		}
		
		#ifdef PRINT_COLLISION_TIME
			interval.stop();
			printf("collisions took %ims\n", (int)interval.getMilliseconds());
		#endif
	}





	std::list<CollisionPair> CollisionManager::getCollisionPairs() const
	{
		std::list<CollisionPair> pairs;
		
		std::list<CollisionPair> prevStaticCollisions;
		std::list<CollisionPair> prevNonstaticCollisions;
		for(auto& collisionPair : previousCollisions) {
			if(collisionPair.collidable1->isStaticCollisionBody() || collisionPair.collidable2->isStaticCollisionBody()) {
				prevStaticCollisions.push_back(collisionPair);
			}
			else {
				prevNonstaticCollisions.push_back(collisionPair);
			}
		}
		
		auto unmatchedPrevCollisions = previousCollisions;
		
		std::list<CollisionPair> staticCollisions;
		std::list<CollisionPair> nonstaticCollisions;
		for(auto it=collidables.begin(); it!=collidables.end(); it++) {
			for(auto it2=std::next(it, 1); it2!=collidables.end(); it2++) {
				auto collidable1 = *it;
				auto collidable2 = *it2;
				CollisionPair pair(collidable1, collidable2);
				auto prevIt = std::find(unmatchedPrevCollisions.begin(), unmatchedPrevCollisions.end(), pair);
				if(prevIt != unmatchedPrevCollisions.end()) {
					unmatchedPrevCollisions.erase(prevIt);
					continue;
				}
				if((!collidable1->isAwake() && !collidable2->isAwake())
				|| (collidable1->isStaticCollisionBody() && collidable2->isStaticCollisionBody())
				|| (!collidable1->awarenessRect.intersects(collidable2->awarenessRect))) {
					continue;
				}
				if(collidable1->isStaticCollisionBody() || collidable2->isStaticCollisionBody()) {
					staticCollisions.push_back(pair);
				}
				else {
					nonstaticCollisions.push_back(pair);
				}
			}
		}
		pairs.splice(pairs.end(), prevStaticCollisions);
		pairs.splice(pairs.end(), staticCollisions);
		pairs.splice(pairs.end(), prevNonstaticCollisions);
		pairs.splice(pairs.end(), nonstaticCollisions);
		return pairs;
	}
	
	
	
	
	
	void CollisionManager::onWillBeginCollisionUpdates(const ApplicationData& appData) {
		// open for implementation
	}
	
	void CollisionManager::onBeginCollisionUpdates(const ApplicationData& appData) {
		// open for implementation
	}
	
	bool CollisionManager::respondsToCollision(const ApplicationData& appData, const Collidable* collidable1, const Collidable* collidable2, CollisionRectPair rectPair, CollisionSide side) const {
		return true;
	}
	
	void CollisionManager::dispatchContactEvents(const ApplicationData& appData, ContactState state, const CollisionPair& pair, const CollisionPair& prevPair) {
		
		auto contactCall1 = [&]() {
			auto collidable1 = pair.collidable1;
			auto collidable2 = pair.collidable2;
			if(collidable1->contactEventListeners.size() > 0) {
				auto contactEvent1 = ContactEvent(collidable1, collidable2, state, pair.getContactingRectPairs(), prevPair.getContactingRectPairs(), pair.ignoredRectPairs, pair.sides, &appData);
				switch(state) {
					case CONTACTSTATE_NEW:
						for(auto listener : collidable1->contactEventListeners) {
							listener->onContact(contactEvent1);
						}
						break;
						
					case CONTACTSTATE_UPDATED:
						for(auto listener : collidable1->contactEventListeners) {
							listener->onContactUpdate(contactEvent1);
						}
						break;
						
					case CONTACTSTATE_FINISHED:
						for(auto listener : collidable1->contactEventListeners) {
							listener->onContactFinish(contactEvent1);
						}
						break;
				}
			}
		};
		
		auto contactCall2 = [&]() {
			auto collidable1 = pair.collidable1;
			auto collidable2 = pair.collidable2;
			if(collidable2->contactEventListeners.size() > 0) {
				auto contactEvent2 = ContactEvent(collidable2, collidable1, state, pair.getReverseContactingRectPairs(), prevPair.getReverseContactingRectPairs(), pair.getReverseIgnoredRectPairs(), pair.getOppositeSides(), &appData);
				switch(state) {
					case CONTACTSTATE_NEW:
						for(auto listener : collidable2->contactEventListeners) {
							listener->onContact(contactEvent2);
						}
						break;
						
					case CONTACTSTATE_UPDATED:
						for(auto listener : collidable2->contactEventListeners) {
							listener->onContactUpdate(contactEvent2);
						}
						break;
						
					case CONTACTSTATE_FINISHED:
						for(auto listener : collidable2->contactEventListeners) {
							listener->onContactFinish(contactEvent2);
						}
						break;
				}
			}
		};
		
		if(pair.collidable1->isStaticCollisionBody()) {
			contactCall2();
			contactCall1();
		}
		else {
			contactCall1();
			contactCall2();
		}
	}
	
	void CollisionManager::dispatchCollisionEvents(const ApplicationData& appData, CollisionState state, CollisionSide side, const CollisionPair& pair, const CollisionPair& prevPair) {
		
		auto collisionCall1 = [&]() {
			auto collidable1 = pair.collidable1;
			auto collidable2 = pair.collidable2;
			if(collidable1->collisionEventListeners.size() > 0) {
				auto collisionEvent1 = CollisionEvent(collidable1, collidable2, side, state, pair.getContactingRectPairs(), prevPair.getContactingRectPairs(), &appData);
				switch(state) {
					case COLLISIONSTATE_NEW:
						for(auto listener : collidable1->collisionEventListeners) {
							listener->onCollision(collisionEvent1);
						}
						break;
						
					case COLLISIONSTATE_UPDATED:
						for(auto listener : collidable1->collisionEventListeners) {
							listener->onCollisionUpdate(collisionEvent1);
						}
						break;
						
					case COLLISIONSTATE_FINISHED:
						for(auto listener : collidable1->collisionEventListeners) {
							listener->onCollisionFinish(collisionEvent1);
						}
						break;
				}
			}
		};
		
		auto collisionCall2 = [&]() {
			auto collidable1 = pair.collidable1;
			auto collidable2 = pair.collidable2;
			if(collidable2->collisionEventListeners.size() > 0) {
				auto collisionEvent2 = CollisionEvent(collidable2, collidable1, CollisionSide_getOpposite(side), state, pair.getReverseContactingRectPairs(), prevPair.getReverseContactingRectPairs(), &appData);
				switch(state) {
					case COLLISIONSTATE_NEW:
						for(auto listener : collidable2->collisionEventListeners) {
							listener->onCollision(collisionEvent2);
						}
						break;
						
					case COLLISIONSTATE_UPDATED:
						for(auto listener : collidable2->collisionEventListeners) {
							listener->onCollisionUpdate(collisionEvent2);
						}
						break;
						
					case COLLISIONSTATE_FINISHED:
						for(auto listener : collidable2->collisionEventListeners) {
							listener->onCollisionFinish(collisionEvent2);
						}
						break;
				}
			}
		};
		
		if(pair.collidable1->isStaticCollisionBody()) {
			collisionCall2();
			collisionCall1();
		}
		else {
			collisionCall1();
			collisionCall2();
		}
	}
	
	void CollisionManager::performFinalCollisionUpdates(const ApplicationData& appData, const CollisionPair& pair, const CollisionPair& prevPair, UpdateData& updateData) {
		// open for implementation
	}
	
	void CollisionManager::onWillFinishCollisionUpdates(const ApplicationData& appData, UpdateData& updateData) {
		// open for implementation
	}
	
	void CollisionManager::onFinishCollisionUpdates(const ApplicationData& appData) {
		// open for implementation
	}
	
	
	
	void CollisionManagerEventListener::onBeginCollisionUpdates(CollisionManager* collisionManager, const ApplicationData& appData) {
		// open for implementation
	}
	
	void CollisionManagerEventListener::onFinishCollisionUpdates(CollisionManager* collisionManager, const ApplicationData& appData) {
		// open for implementation
	}
}
