
#pragma once

#include <cstddef>

#ifndef ARRAYLIST_DEFAULT_PREALLOC_COUNT
#define ARRAYLIST_DEFAULT_PREALLOC_COUNT 0
#endif

namespace fgl
{
	//Actor
	class Actor;
		class SpriteActor;
		class TextActor;
		class WireframeActor;
	
	//Animation
	class Animation;
	class AnimationPlayer;
	class AnimationProvider;
	
	//Application
	class Application;
	class ApplicationData;
	class BatchLoader;
	class EventManager;
	
	//Audio
	class Music;
	class Sound;
	
	//Event
	class Event;
	
	//Exception
	class Exception;
		class BadCastException;
			class BadAnyCastException;
			class BadNumberCastException;
		class IllegalArgumentException;
			class NumberFormatException;
		class IllegalNumberOperationException;
		class IllegalReturnValueException;
		class IllegalStateException;
		class InitializeLibraryException;
		class NotImplementedException;
		class OutOfBoundsException;
			class ArrayListOutOfBoundsException;
			class DataOutOfBoundsException;
			class DictionaryKeyNotFoundException;
			class ImageOutOfBoundsException;
			class StringOutOfBoundsException;
		class RenderGlyphException;
		class ScreenNavigationException;
		class TextureImageCreateException;
		class TextureImageUpdateException;
		class UnsupportedImageFormatException;
	
	//Graphics
	class Graphics;
	class Color;
	class Image;
	class TextureImage;
	
	//Input
	class Keyboard;
	class Mouse;
	class Multitouch;
	
	//IO
	class Console;
	class FileTools;
	
	//Network
	class NetworkProtocol;
	
	//Physics
	class Collidable;
	class CollisionManager;
	class CollisionPair;
	class CollisionRectBuilder;
		//Event
		class CollisionEvent;
		class ContactEvent;
		//CollisionRects
		class CollisionRect;
			class BoxCollisionRect;
			class PixelCollisionRect;
			class PolygonCollisionRect;
	
	//Screen
		//Transition
		class Transition;
			class FadeColorTransition;
			class FadeZoomTransition;
			class PopoverTransition;
			class SlideTransition;
	class ScreenElement;
		//UI
		class ActorMenuElement;
		class AnimationElement;
		class ButtonElement;
		class CheckboxElement;
		class DraggableElement;
		class GridSelectorElement;
		class ImageElement;
		class SegmentedSelectorElement;
		class TextElement;
		class TextInputElement;
		class TouchElement;
		class ZoomPanElement;
	class Screen;
		class MenuScreen;
		class PopupScreen;
		class ScreenManager;
	
	//Utilities
		//Font
		class Font;
		//Time
		class DateTime;
		class TimeInterval;
	class Any;
	template<typename T, size_t PREALLOC_COUNT=ARRAYLIST_DEFAULT_PREALLOC_COUNT>
	class ArrayList;
	class Aspect;
	template<typename ASPECT>
	class Aspectable;
	template<typename KEY_TYPE, typename VALUE_TYPE>
	class BasicDictionary;
	template<typename CHAR_TYPE>
	class BasicString;
	class Data;
	class Math;
	class Number;
	class Plist;
	template<typename T>
	class Promise;
	template<typename T>
	class Range;
	class Retainable;
	class Thread;
	
	//Window
	class AssetManager;
	class Viewport;
	class Window;
	
	typedef signed char Int8;
	typedef unsigned char Uint8;
	
	typedef signed short Int16;
	typedef unsigned short Uint16;
	
	typedef signed int Int32;
	typedef unsigned int Uint32;
	
	#if defined(_MSC_VER)
		typedef signed __int64 Int64;
		typedef unsigned __int64 Uint64;
	#else
		typedef signed long long Int64;
		typedef unsigned long long Uint64;
	#endif
	
	typedef Uint8 byte;
	typedef bool boolean;
	
//#define nullptr null
}
