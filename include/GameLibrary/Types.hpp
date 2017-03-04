
#pragma once

namespace fgl
{
	//Actor
	class Actor;
		class SpriteActor;
		class Animation;
		class TextActor;
		class WireframeActor;
	
	//Application
	class Application;
	class ApplicationData;
	class BatchLoader;
	class EventManager;
	
	//Audio
	class Music;
	class Sound;
	
	//Exception
	class Exception;
		class BadCastException;
			class BadAnyCastException;
			class BadNumberCastException;
		class IllegalArgumentException;
			class NumberFormatException;
		class IllegalNumberOperationException;
		class IllegalStateException;
		class InitializeLibraryException;
		class NotImplementedException;
		class OutOfBoundsException;
			class ArrayListOutOfBoundsException;
			class BitListOutOfBoundsException;
			class BitSetOutOfBoundsException;
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
		class ImageElement;
		class ZoomPanElement;
		class TextInputElement;
	class Screen;
		class MenuScreen;
		class ScreenManager;
	
	//Utilities
		//Font
		class Font;
		//Time
		class DateTime;
		class TimeInterval;
	class Any;
	template<typename T>
	class ArrayList;
	template<typename KEY_TYPE, typename VALUE_TYPE>
	class BasicDictionary;
	template<typename CHAR_TYPE>
	class BasicString;
	class Data;
	class Math;
	template<typename T1, typename T2>
	class Pair;
	class Plist;
	class Number;
	class Thread;
	
	//Window
	class AssetManager;
	class View;
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
