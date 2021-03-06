
#include <GameLibrary/Screen/UI/ActorMenuElement.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	ActorMenuElement::ActorMenuElement(const RectangleD&frame) : ScreenElement(frame)
	{
		selectedIndex = ActorMenuElement::NO_SELECTION;
		keyboardEnabled = false;
		pressingActor = false;
	}
	
	ActorMenuElement::~ActorMenuElement()
	{
		for(auto actorLayoutMgr : actorLayoutMgrs) {
			delete actorLayoutMgr;
		}
		for(auto actor : actors) {
			delete actor;
		}
	}
	
	void ActorMenuElement::update(ApplicationData appData)
	{
		ScreenElement::update(appData);
		ArrayList<std::pair<Keyboard::Key, KeyDirection> > keyList = keys;
		for(size_t i=0; i<keyList.size(); i++)
		{
			std::pair<Keyboard::Key, KeyDirection>& keypair = keyList.get(i);
			if(Keyboard::isKeyPressed(keypair.first) && !Keyboard::wasKeyPressed(keypair.first))
			{
				//on key press
				switch(keypair.second)
				{
					case KEYDIR_UP:
					moveHoverUp();
					break;
					
					case KEYDIR_DOWN:
					moveHoverDown();
					break;
					
					case KEYDIR_LEFT:
					moveHoverLeft();
					break;
					
					case KEYDIR_RIGHT:
					moveHoverRight();
					break;
					
					case KEYDIR_SELECT:
					if(keyboardEnabled && selectedIndex!=ActorMenuElement::NO_SELECTION)
					{
						pressingActor = true;
						onActorPress(selectedIndex);
					}
					break;
				}
			}
			else if(Keyboard::wasKeyPressed(keypair.first) && !Keyboard::isKeyPressed(keypair.first))
			{
				if(keypair.second==KEYDIR_SELECT)
				{
					if(keyboardEnabled && pressingActor)
					{
						onActorRelease(selectedIndex);
						selectCurrentIndex();
					}
				}
			}
		}
		
		if(keyboardEnabled)
		{
			size_t lastSelectedIndex = selectedIndex;
			if(lastSelectedIndex != ActorMenuElement::NO_SELECTION)
			{
				Actor* actor = actors.get(lastSelectedIndex);
				actor->update(appData);
				if(actor->isMouseOver() && !actor->wasMouseOver())
				{
					setKeyboardEnabled(false);
				}
				else if(actor->isMousePressed() && !actor->wasMousePressed())
				{
					setKeyboardEnabled(false);
				}
			}
			for(size_t i=0; i<actors.size(); i++)
			{
				if(i != lastSelectedIndex)
				{
					Actor* actor = actors.get(i);
					actor->update(appData);
					if(actor->isMouseOver() && !actor->wasMouseOver())
					{
						setKeyboardEnabled(false);
					}
					else if(actor->isMousePressed() && !actor->wasMousePressed())
					{
						setKeyboardEnabled(false);
					}
				}
			}
		}
		else
		{
			size_t lastSelectedIndex = selectedIndex;
			//test/update currently hovered actor
			if(lastSelectedIndex != ActorMenuElement::NO_SELECTION)
			{
				Actor* actor = actors.get(lastSelectedIndex);
				actor->update(appData);
				if(actor->isMouseOver())
				{
					if(pressingActor)
					{
						if(!actor->isMousePressed())
						{
							pressingActor = false;
							//actor->clearMouseState();
							onActorRelease(lastSelectedIndex);
							//onActorHoverFinish(lastSelectedIndex);
							onActorSelect(lastSelectedIndex);
						}
					}
					else
					{
						if(actor->isMousePressed())
						{
							pressingActor = true;
							onActorPress(lastSelectedIndex);
						}
					}
				}
				else
				{
					selectedIndex = ActorMenuElement::NO_SELECTION;
					if(pressingActor)
					{
						pressingActor = false;
						if(actor->didMouseRelease())
						{
							//TODO have mouse state clearing be called from menuscreen subclass
							//actor->clearMouseState();
							onActorRelease(lastSelectedIndex);
							onActorHoverFinish(lastSelectedIndex);
							onActorSelect(lastSelectedIndex);
						}
						else
						{
							onActorPressCancel(lastSelectedIndex);
							onActorHoverFinish(lastSelectedIndex);
						}
					}
					else
					{
						onActorHoverFinish(lastSelectedIndex);
					}
				}
			}

			ArrayList<Actor*> menuActors = actors;

			//test/update other actors
			for(size_t i=0; i<menuActors.size(); i++)
			{
				if(i != lastSelectedIndex)
				{
					Actor* actor = menuActors.get(i);

					//make sure actor wasn't added or removed from the screen during an event
					size_t actorIndex = ActorMenuElement::NO_SELECTION;
					if(i<actors.size() && actors.get(i)==actor)
					{
						actorIndex = i;
					}
					else
					{
						actorIndex = actors.indexOf(actor);
					}
					
					//if the actor is still in the screen
					if(actorIndex!=ArrayList<Actor*>::NOT_FOUND)
					{
						actor->update(appData);
						
						//checking selection by mouse
						if(!keyboardEnabled)
						{
							if(selectedIndex == ActorMenuElement::NO_SELECTION)
							{
								if(actor->isMouseOver())
								{
									//actor->wasMouseOver() is implied to be false, since we're testing for objects that are not the current value of MenuScreen::selectedIndex
									
									//make sure actor wasn't added or removed from the screen during an event
									if(actorIndex<actors.size() && actors.get(actorIndex)==actor)
									{
										selectedIndex = actorIndex;
									}
									else
									{
										actorIndex = actors.indexOf(actor);
									}
									
									//check item's mouse hover/press state
									if(actorIndex != ActorMenuElement::NO_SELECTION)
									{
										selectedIndex = actorIndex;
										onActorHover(selectedIndex);
										
										//make sure actor wasn't added or removed from the screen during an event
										if(actorIndex<actors.size() && actors.get(actorIndex)==actor)
										{
											selectedIndex = actorIndex;
										}
										else
										{
											actorIndex = actors.indexOf(actor);
										}
										
										//check item's mouse press state
										if(actorIndex!=ArrayList<Actor*>::NOT_FOUND && selectedIndex==actorIndex && !keyboardEnabled)
										{
											if(actor->didMousePress() && !actor->wasMousePressed())
											{
												pressingActor = true;
												onActorPress(selectedIndex);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	void ActorMenuElement::onLayoutChildElements()
	{
		ScreenElement::onLayoutChildElements();
		ScreenElement* parentElement = getParentElement();
		if(parentElement!=nullptr)
		{
			RectangleD parentFrame = parentElement->getFrame();
			for(size_t actors_size=actors.size(), i=0; i<actors_size; i++)
			{
				Actor* actor = actors[i];
				AutoLayoutManager& layoutMgr = *actorLayoutMgrs[i];
				actor->scaleToFit(layoutMgr.calculateFrame(actor->getFrame(), parentFrame));
			}
		}
	}
	
	void ActorMenuElement::drawActor(ApplicationData appData, Graphics graphics, Actor*actor) const
	{
		actor->draw(appData, graphics);
	}
	
	void ActorMenuElement::drawMain(ApplicationData appData, Graphics graphics) const
	{
		size_t hoveredIndex = selectedIndex;
		for(size_t i=0; i<actors.size(); i++)
		{
			if(i!=hoveredIndex)
			{
				drawActor(appData, graphics, actors.get(i));
			}
		}
		
		if(hoveredIndex != ActorMenuElement::NO_SELECTION)
		{
			drawActor(appData, graphics, actors.get(hoveredIndex));
		}
	}
	
	size_t ActorMenuElement::addActor(Actor*actor)
	{
		if(actor == nullptr)
		{
			throw IllegalArgumentException("actor", "null");
		}
		actors.add(actor);
		AutoLayoutManager* layoutMgr = new AutoLayoutManager();
		layoutMgr->setOffsetByContainer(true);
		actorLayoutMgrs.add(layoutMgr);
		actor->scaleToFit(layoutMgr->calculateFrame(actor->getFrame(), getFrame()));
		return actors.size()-1;
	}
	
	size_t ActorMenuElement::addActor(const RectD&bounds, Actor*actor)
	{
		if(actor == nullptr)
		{
			throw IllegalArgumentException("actor", "null");
		}
		actors.add(actor);
		AutoLayoutManager* layoutMgr = new AutoLayoutManager();
		layoutMgr->setRule(LAYOUTRULE_LEFT, bounds.left, LAYOUTVALUE_RATIO);
		layoutMgr->setRule(LAYOUTRULE_TOP, bounds.top, LAYOUTVALUE_RATIO);
		layoutMgr->setRule(LAYOUTRULE_RIGHT, 1.0-bounds.right, LAYOUTVALUE_RATIO);
		layoutMgr->setRule(LAYOUTRULE_BOTTOM, 1.0-bounds.bottom, LAYOUTVALUE_RATIO);
		layoutMgr->setOffsetByContainer(true);
		actorLayoutMgrs.add(layoutMgr);
		actor->scaleToFit(layoutMgr->calculateFrame(actor->getFrame(), getFrame()));
		return actors.size()-1;
	}
	
	Actor* ActorMenuElement::getActor(size_t index) const
	{
		return actors.get(index);
	}
	
	const ArrayList<Actor*>& ActorMenuElement::getActors() const
	{
		return actors;
	}
	
	void ActorMenuElement::removeActor(Actor*actor)
	{
		size_t index = actors.indexOf(actor);
		if(index != ArrayList<Actor*>::NOT_FOUND)
		{
			removeActor(index);
		}
	}
	
	void ActorMenuElement::removeActor(size_t index)
	{
		actors.remove(index);
		AutoLayoutManager* layoutMgr = actorLayoutMgrs.get(index);
		actorLayoutMgrs.remove(index);
		delete layoutMgr;
		if(selectedIndex == index)
		{
			selectedIndex = ActorMenuElement::NO_SELECTION;
		}
		else if(selectedIndex > index)
		{
			if(actors.size()>0)
			{
				selectedIndex--;
			}
			else
			{
				selectedIndex = ActorMenuElement::NO_SELECTION;
			}
		}
	}
	
	size_t ActorMenuElement::getTotalActors()
	{
		return actors.size();
	}
	
	void ActorMenuElement::onActorHover(size_t index)
	{
		//
	}
	
	void ActorMenuElement::onActorHoverFinish(size_t index)
	{
		//
	}
	
	void ActorMenuElement::onActorPress(size_t index)
	{
		//
	}
	
	void ActorMenuElement::onActorPressCancel(size_t index)
	{
		//
	}
	
	void ActorMenuElement::onActorRelease(size_t index)
	{
		//
	}
	
	void ActorMenuElement::onActorSelect(size_t index)
	{
		//
	}
	
	
	void ActorMenuElement::moveHoverUp()
	{
		size_t index = getSelectedIndex();
		size_t totalItems = getTotalActors();
		if(index == ActorMenuElement::NO_SELECTION)
		{
			if(totalItems>0)
			{
				index = totalItems-1;
				setSelectedIndex(index);
			}
		}
		else
		{
			if(index==0)
			{
				if(totalItems>0)
				{
					index = totalItems-1;
				}
				else
				{
					index = ActorMenuElement::NO_SELECTION;
				}
			}
			else
			{
				index--;
			}
			
			setSelectedIndex(index);
		}
	}
	
	void ActorMenuElement::moveHoverDown()
	{
		size_t index = getSelectedIndex();
		size_t totalActors = getTotalActors();
		if(index == ActorMenuElement::NO_SELECTION)
		{
			if(totalActors>0)
			{
				index = 0;
				setSelectedIndex(index);
			}
		}
		else
		{
			index++;
			if(index>totalActors)
			{
				if(totalActors>0)
				{
					index = 0;
				}
				else
				{
					index = ActorMenuElement::NO_SELECTION;
				}
			}
			
			setSelectedIndex(index);
		}
	}
	
	void ActorMenuElement::moveHoverLeft()
	{
		moveHoverUp();
	}
	
	void ActorMenuElement::moveHoverRight()
	{
		moveHoverUp();
	}
	
	bool ActorMenuElement::selectCurrentIndex()
	{
		if(selectedIndex!=ActorMenuElement::NO_SELECTION && selectedIndex < actors.size())
		{
			onActorSelect(selectedIndex);
			return true;
		}
		return false;
	}
	
	void ActorMenuElement::addUpKey(const Keyboard::Key&key)
	{
		for(size_t i=0; i<keys.size(); i++)
		{
			std::pair<Keyboard::Key, KeyDirection>& cmp = keys.get(i);
			if(cmp.first == key)
			{
				keys.remove(i);
				i = keys.size();
			}
		}
		keys.add(std::pair<Keyboard::Key, KeyDirection>(key, KEYDIR_UP));
	}
	
	void ActorMenuElement::addDownKey(const Keyboard::Key&key)
	{
		for(size_t i=0; i<keys.size(); i++)
		{
			std::pair<Keyboard::Key, KeyDirection>& cmp = keys.get(i);
			if(cmp.first == key)
			{
				keys.remove(i);
				i = keys.size();
			}
		}
		keys.add(std::pair<Keyboard::Key, KeyDirection>(key, KEYDIR_DOWN));
	}
	
	void ActorMenuElement::addLeftKey(const Keyboard::Key&key)
	{
		for(size_t i=0; i<keys.size(); i++)
		{
			std::pair<Keyboard::Key, KeyDirection>& cmp = keys.get(i);
			if(cmp.first == key)
			{
				keys.remove(i);
				i = keys.size();
			}
		}
		keys.add(std::pair<Keyboard::Key, KeyDirection>(key, KEYDIR_LEFT));
	}
	
	void ActorMenuElement::addRightKey(const Keyboard::Key&key)
	{
		for(size_t i=0; i<keys.size(); i++)
		{
			std::pair<Keyboard::Key, KeyDirection>& cmp = keys.get(i);
			if(cmp.first == key)
			{
				keys.remove(i);
				i = keys.size();
			}
		}
		keys.add(std::pair<Keyboard::Key, KeyDirection>(key, KEYDIR_RIGHT));
	}
	
	void ActorMenuElement::addSelectKey(const Keyboard::Key&key)
	{
		for(size_t i=0; i<keys.size(); i++)
		{
			std::pair<Keyboard::Key, KeyDirection>& cmp = keys.get(i);
			if(cmp.first == key)
			{
				keys.remove(i);
				i = keys.size();
			}
		}
		keys.add(std::pair<Keyboard::Key, KeyDirection>(key, KEYDIR_SELECT));
	}
	
	void ActorMenuElement::clearKeys()
	{
		keys.clear();
	}
	
	void ActorMenuElement::setKeyboardEnabled(bool toggle)
	{
		if((toggle && !keyboardEnabled) || (keyboardEnabled && selectedIndex==ActorMenuElement::NO_SELECTION))
		{
			//enable keyboard
			keyboardEnabled = true;
			if(pressingActor)
			{
				pressingActor = false;
				onActorPressCancel(selectedIndex);
			}
		}
		else if(!toggle && keyboardEnabled)
		{
			//disable keyboard
			keyboardEnabled = false;
			if(pressingActor)
			{
				pressingActor = false;
				onActorPressCancel(selectedIndex);
			}
			if(selectedIndex != ActorMenuElement::NO_SELECTION)
			{
				Actor* actor = actors.get(selectedIndex);
				if(!actor->isMouseOver())
				{
					onActorHoverFinish(selectedIndex);
				}
			}
		}
	}
	
	void ActorMenuElement::clearMouseStates()
	{
		if(selectedIndex != ActorMenuElement::NO_SELECTION)
		{
			Actor*actor = actors.get(selectedIndex);
			actor->clearMouseState();
			size_t oldSelectedIndex = selectedIndex;
			if(pressingActor)
			{
				onActorPressCancel(oldSelectedIndex);
			}
			selectedIndex = ActorMenuElement::NO_SELECTION;
			onActorHoverFinish(oldSelectedIndex);
		}
	}
	
	void ActorMenuElement::setSelectedIndex(size_t index)
	{
		bool keyboardWasEnabled = keyboardEnabled;
		setKeyboardEnabled(true);
		if(index != selectedIndex)
		{
			size_t oldSelectedIndex = selectedIndex;
			bool wasPressingActor = pressingActor;
			pressingActor = false;
			
			if(oldSelectedIndex != ActorMenuElement::NO_SELECTION)
			{
				if(wasPressingActor)
				{
					onActorPressCancel(oldSelectedIndex);
				}
				selectedIndex = ActorMenuElement::NO_SELECTION;
				onActorHoverFinish(oldSelectedIndex);
			}
			
			if(index != ActorMenuElement::NO_SELECTION && index<actors.size())
			{
				selectedIndex = index;
				onActorHover(index);
				if(wasPressingActor && keyboardWasEnabled)
				{
					pressingActor = true;
					onActorPress(index);
				}
			}
		}
	}
	
	bool ActorMenuElement::isKeyboardEnabled() const
	{
		return keyboardEnabled;
	}
	
	size_t ActorMenuElement::getSelectedIndex() const
	{
		return selectedIndex;
	}
	
	AutoLayoutManager* ActorMenuElement::getActorAutoLayoutManager(size_t index) const
	{
		return actorLayoutMgrs.get(index);
	}
}
