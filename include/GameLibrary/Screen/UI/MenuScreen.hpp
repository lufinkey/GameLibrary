
#pragma once

#include <climits>
#include <GameLibrary/Actor/Actor.hpp>
#include <GameLibrary/Actor/SpriteActor.hpp>
#include <GameLibrary/Actor/TextActor.hpp>
#include <GameLibrary/Animation/Animation.hpp>
#include <GameLibrary/Graphics/Graphics.hpp>
#include <GameLibrary/Input/Keyboard.hpp>
#include <GameLibrary/Screen/Screen.hpp>
#include "ActorMenuElement.hpp"

namespace fgl
{
	/*! Thin Screen wrapper around ActorMenuElement. The main difference is that this class DOES automatically handle deallocation of any added Actor pointers.*/
	class MenuScreen : public Screen
	{
	public:
		/*! The selected index when a MenuScreen has not selected an item*/
		static constexpr size_t NO_SELECTION = (size_t)-1;
		
		
		/*! default constructor*/
		MenuScreen();
		/*! Constructs a MenuScreen to be the root Screen of a Window. Only one screen in any Screen stack should be delegated as the root Screen.
			\param window the Window that the Screen will be contained within*/
		explicit MenuScreen(Window* window);
		
		
		MenuScreen(const MenuScreen&) = delete;
		MenuScreen& operator=(const MenuScreen&) = delete;
		
		
		/*! Adds a SpriteActor. This Actor's deallocation will be automatically handled when this Screen is deallocated
			\param actor a SpriteActor pointer
			\returns the index of the added SpriteActor
			\throws fgl::IllegalArgumentException if actor argument is null*/
		size_t addItem(SpriteActor* actor);
		/*! Adds a SpriteActor that will be automatically resized by the AutoLayout. This Actor's deallocation will be automatically handled when this Screen is deallocated
			\param bounds the ratio in the frame where the Actor will be laid out
			\param actor a SpriteActor pointer
			\returns the index of the added SpriteActor
			\throws fgl::IllegalArgumentException if actor argument is null*/
		size_t addItem(const RectD& bounds, SpriteActor* actor);
		/*! Creates and adds a SpriteActor.
			\param position the position of the Actor in the Screen
			\param animation the default Animation of the Actor
			\param direction the default Animation::Direction of the Actor
			\param destructAnimation true to have the Actor handle deallocation of the Animation, or false to manually handle deallocation of the Animation
		 	\returns the index of the added item
			\throws fgl::IllegalArgumentException if the animation argument is null, or if direction is not a valid Animation::Direction*/
		size_t addItem(const Vector2d& position, Animation* animation, const Animation::Direction& direction=Animation::Direction::FORWARD, bool destructAnimation=true);
		/*! Creates and adds a SpriteActor that will be automatically resized by the AutoLayout.
			\param bounds the ratio in the frame where the Actor will be laid out
			\param animation the default Animation of the Actor
			\param direction the default Animation::Direction of the Actor
			\param destructAnimation true to have the Actor handle deallocation of the Animation, or false to manually handle deallocation of the Animation
		 	\returns the index of the added item
			\throws fgl::IllegalArgumentException if the animation argument is null, or if direction is not a valid Animation::Direction*/
		size_t addItem(const RectD& bounds, Animation* animation, const Animation::Direction& direction=Animation::Direction::FORWARD, bool destructAnimation=true);
		/*! Adds a TextActor. This Actor's deallocation will be automatically handled when this Screen is deallocated.
			\param actor a TextActor pointer
		 	\returns the index of the added item
			\throws fgl::IllegalArgumentException if actor argument is null*/
		size_t addItem(TextActor* actor);
		/*! Adds a TextActor that will be automatically resized by the AutoLayout. This Actor's deallocation will be automatically handled when this Screen is deallocated.
			\param bounds the ratio in the frame where the Actor will be laid out
			\param actor a TextActor pointer
		 	\returns the index of the added item
			\throws fgl::IllegalArgumentException if actor argument is null*/
		size_t addItem(const RectD& bounds, TextActor* actor);
		/*! Creates and adds a TextActor.
			\param position the position of the Actor in the Screen
			\param text a string to display
			\param font a font to use to display the text
			\param color the text color
			\param fontSize the glyph size of the font
			\param fontStyle the style (plain, bold italic, etc.) of the font \see fgl::Font::Style
			\param alignment the alignment of the text, relative the origin \see fgl::TextActor::TextAlignment
			\returns the index of the added item*/
		size_t addItem(const Vector2d& position, const String& text, Font* font=Graphics::getDefaultFont(), const Color& color=Colors::BLACK, unsigned int fontSize=18, int fontStyle=Font::STYLE_PLAIN, const TextActor::TextAlignment& alignment=TextActor::ALIGN_CENTER);
		/*! Creates and adds a TextActor that will be automatically resized by the AutoLayout.
			\param bounds the ratio in the frame where the Actor will be laid out
			\param text a string to display
			\param font a font to use to display the text
			\param color the text color
			\param fontSize the glyph size of the font
			\param fontStyle the style (plain, bold italic, etc.) of the font \see fgl::Font::Style
			\param alignment the alignment of the text, relative the origin \see fgl::TextActor::TextAlignment
		 	\returns the index of the added item*/
		size_t addItem(const RectD& bounds, const String& text, Font* font=Graphics::getDefaultFont(), const Color& color=Colors::BLACK, unsigned int fontSize=18, int fontStyle=Font::STYLE_PLAIN, const TextActor::TextAlignment& alignment=TextActor::ALIGN_CENTER);
		/*! Removes and deallocates a previously added Actor pointer.
			\param index the index of the Actor. When an Actor is removed, the indexes of other Actors may change*/
		void removeItem(size_t index);
		/*! Gets the total number of items stored in the menu.
			\returns an unsigned int representing the total amount of Actor pointers added to the menu*/
		size_t getTotalItems();
		/*! Gets an Actor pointer at a given index.
			\param index the index of the Actor
			\returns an Actor pointer*/
		Actor* getItem(size_t index) const;
		
		
		/*! Called when an Actor in the menu is hovered over by a mouse or touch instance.
			\param index the index of the Actor*/
		virtual void onItemHover(size_t index);
		/*! Called when an Actor in the menu finished being hovered over by a mouse or touch instance.
			\param index the index of the Actor*/
		virtual void onItemHoverFinish(size_t index);
		/*! Called when an Actor in the menu is pressed by a mouse or touch instance.
			\param index the index of the Actor*/
		virtual void onItemPress(size_t index);
		/*! Called when an Actor in the menu has a pressed mouse or touch instance leave the vicinity of the Actor, or some other event cancels the press.
			\param index the index of the Actor*/
		virtual void onItemPressCancel(size_t index);
		/*! Called when an Actor in the menu is released by a mouse or touch instance that previously pressed it.
			\param index the index of the Actor*/
		virtual void onItemRelease(size_t index);
		/*! Called after an Actor in the menu is released. Menu button actions should be handled here.
			\param index the index of the Actor*/
		virtual void onItemSelect(size_t index);
		
		
		/*! Manually sets the current hover to the next Actor above the current one, or the first Actor in the list, if no Actor is currently hovered.
		This function automatically sets the Keyboard to be the enabled input for this menu.*/
		virtual void moveHoverUp();
		/*! Manually sets the current hover to the next Actor below the current one, or the last Actor in the list, if no Actor is currently hovered.
		This function automatically sets the Keyboard to be the enabled input for this menu.*/
		virtual void moveHoverDown();
		/*! Manually sets the current hover to the next Actor to the left of the current one, or the first Actor in the list, if no Actor is currently hovered.
		This function automatically sets the Keyboard to be the enabled input for this menu.*/
		virtual void moveHoverLeft();
		/*! Manually sets the current hover to the next Actor to the right of the current one, or the last Actor in the list, if no Actor is currently hovered.
		This function automatically sets the Keyboard to be the enabled input for this menu.*/
		virtual void moveHoverRight();
		/*! Calls onActorSelect(unsigned int) for the currently hovered Actor, or does nothing if no Actor is currently hovered.
			\returns true if an Actor was selected, or false if otherwise*/
		virtual bool selectCurrentIndex();
		
		
		/*! Adds a keyboard key to manually move up in the menu.
			\param key a key constant*/
		void addUpKey(const Keyboard::Key& key);
		/*! Adds a keyboard key to manually move down in the menu.
			\param key a key constant*/
		void addDownKey(const Keyboard::Key& key);
		/*! Adds a keyboard key to manually move left in the menu.
			\param key a key constant*/
		void addLeftKey(const Keyboard::Key& key);
		/*! Adds a keyboard key to manually move right in the menu.
			\param key a key constant*/
		void addRightKey(const Keyboard::Key& key);
		/*! Adds a keyboard key to manually select the current menu item.
			\param key a key constant*/
		void addSelectKey(const Keyboard::Key& key);
		/*! Removes all the added keys from the menu.*/
		void clearKeys();
		
		
		/*! Enables keyboard input instead of mouse or touch input.
		The keyboard is automatically enabled if one of the added keys is pressed, and automatically disabled if one of the Actor objects becomes hovered or pressed.
			\param enabled true to enable keyboard input, false to disable keyboard input*/
		void setKeyboardEnabled(bool enabled);
		/*! Clears all the current mouse states for all the Actor objects, and cancels all press states.*/
		void clearMouseStates();
		/*! Sets the currently hovered Actor index in the menu. This automatically enables Keyboard input.
			\param index the Actor index to set hovered, or MENUSCREEN_NOSELECTION for no selected index*/
		void setSelectedIndex(size_t index);
		
		
		/*! Tells if the menu's input is currently set to the keyboard.
			\returns true if input is set to the keyboard, and false if input is set to mouse or touch*/
		bool isKeyboardEnabled() const;
		/*! Gets the currently hovered Actor index in the menu.
			\returns an index for an Actor in the menu, or MENUSCREEN_NOSELECTION if no index is selected*/
		size_t getSelectedIndex() const;
		
		
		/*! Gets the automatic layout manager for the actor at the specified index.
			\returns an AutoLayoutManager pointer*/
		AutoLayoutManager* getActorAutoLayoutManager(size_t index) const;
		
	protected:
		/*! Called to draw an Actor in the menu. This function can be overridden to add custom drawing behavior.
			\param appData specifies information about the Application drawing the Actor, such as the Window object, the Viewport transform, etc. \see fgl::ApplicationData
			\param graphics the Graphics object used to draw the Actor
			\param item the Actor from the menu being drawn*/
		virtual void drawItem(ApplicationData appData, Graphics graphics, Actor* item) const;
		
	private:
		class MainElement : public ActorMenuElement
		{
		private:
			MenuScreen*menuScreen;
			
		protected:
			virtual void drawActor(ApplicationData appData, Graphics graphics, Actor* actor) const override;
			
		public:
			MainElement(MenuScreen* menuScreen, const RectangleD& frame);
			
			virtual void onActorHover(size_t index) override;
			virtual void onActorHoverFinish(size_t index) override;
			virtual void onActorPress(size_t index) override;
			virtual void onActorPressCancel(size_t index) override;
			virtual void onActorRelease(size_t index) override;
			virtual void onActorSelect(size_t index) override;
		};
		
		MainElement* mainElement;
	};
}
