
#pragma once

#include "Actor/Actor.hpp"
#include "Actor/SpriteActor.hpp"
#include "Actor/TextActor.hpp"
#include "Actor/WireframeActor.hpp"

#include "Animation/Animation.hpp"
#include "Animation/AnimationPlayer.hpp"
#include "Animation/AnimationProvider.hpp"

#include "Application/Application.hpp"
#include "Application/ApplicationData.hpp"
#include "Application/BatchLoader.hpp"

#include "Audio/Music.hpp"
#include "Audio/Sound.hpp"

#include "Draw/DrawContext.hpp"
#include "Draw/DrawManager.hpp"
#include "Draw/Drawable.hpp"

#include "Event/Event.hpp"

#include "Exception/Exception.hpp"
#include "Exception/BadCastException.hpp"
#include "Exception/IllegalArgumentException.hpp"
#include "Exception/IllegalReturnValueException.hpp"
#include "Exception/IllegalStateException.hpp"
#include "Exception/NotImplementedException.hpp"
#include "Exception/NumberFormatException.hpp"
#include "Exception/OutOfBoundsException.hpp"
#include "Exception/Graphics/ImageOutOfBoundsException.hpp"
#include "Exception/Graphics/TextureImageCreateException.hpp"
#include "Exception/Graphics/TextureImageUpdateException.hpp"
#include "Exception/Graphics/UnsupportedImageFormatException.hpp"
#include "Exception/Screen/ScreenNavigationException.hpp"
#include "Exception/Utilities/ArrayListOutOfBoundsException.hpp"
#include "Exception/Utilities/BadAnyCastException.hpp"
#include "Exception/Utilities/DataOutOfBoundsException.hpp"
#include "Exception/Utilities/DictionaryKeyNotFoundException.hpp"
#include "Exception/Utilities/IllegalNumberOperationException.hpp"
#include "Exception/Utilities/StringOutOfBoundsException.hpp"
#include "Exception/Utilities/Font/RenderGlyphException.hpp"

#include "Graphics/Color.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Image.hpp"
#include "Graphics/PixelIterator.hpp"
#include "Graphics/TextureImage.hpp"

#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"
#include "Input/Multitouch.hpp"

#include "IO/Console.hpp"
#include "IO/FileTools.hpp"

#include "Network/NetworkProtocol.hpp"

#include "Physics/Collidable.hpp"
#include "Physics/CollisionEvent.hpp"
#include "Physics/CollisionManager.hpp"
#include "Physics/CollisionPair.hpp"
#include "Physics/CollisionRectBuilder.hpp"
#include "Physics/CollisionSide.hpp"
#include "Physics/ContactEvent.hpp"
#include "Physics/CollisionRects/CollisionRect.hpp"
#include "Physics/CollisionRects/BoxCollisionRect.hpp"
#include "Physics/CollisionRects/PixelCollisionRect.hpp"
#include "Physics/CollisionRects/PolygonCollisionRect.hpp"

#include "Screen/AutoLayoutManager.hpp"
#include "Screen/ScreenElement.hpp"
#include "Screen/Screen.hpp"
#include "Screen/ScreenManager.hpp"
#include "Screen/Transition/Transition.hpp"
#include "Screen/Transition/FadeColorTransition.hpp"
#include "Screen/Transition/FadeZoomTransition.hpp"
#include "Screen/Transition/PopoverTransition.hpp"
#include "Screen/Transition/SlideTransition.hpp"
#include "Screen/UI/ActorMenuElement.hpp"
#include "Screen/UI/AnimationElement.hpp"
#include "Screen/UI/ButtonElement.hpp"
#include "Screen/UI/CheckboxElement.hpp"
#include "Screen/UI/DraggableElement.hpp"
#include "Screen/UI/GridSelectorElement.hpp"
#include "Screen/UI/ImageElement.hpp"
#include "Screen/UI/MenuScreen.hpp"
#include "Screen/UI/PopupScreen.hpp"
#include "Screen/UI/SegmentedSelectorElement.hpp"
#include "Screen/UI/TextElement.hpp"
#include "Screen/UI/TextInputElement.hpp"
#include "Screen/UI/ZoomPanElement.hpp"

#include "Utilities/Any.hpp"
#include "Utilities/ArrayList.hpp"
#include "Utilities/Aspectable.hpp"
#include "Utilities/Data.hpp"
#include "Utilities/Dictionary.hpp"
#include "Utilities/Math.hpp"
#include "Utilities/Number.hpp"
#include "Utilities/PerformanceMacros.hpp"
#include "Utilities/PlatformChecks.hpp"
#include "Utilities/Plist.hpp"
#include "Utilities/Promise.hpp"
#include "Utilities/Range.hpp"
#include "Utilities/Retainable.hpp"
#include "Utilities/String.hpp"
#include "Utilities/Thread.hpp"
#include "Utilities/Tools.hpp"
#include "Utilities/Direction/OctalDirection.hpp"
#include "Utilities/Direction/QuadDirection.hpp"
#include "Utilities/Font/Font.hpp"
#include "Utilities/Geometry/Circle.hpp"
#include "Utilities/Geometry/Line.hpp"
#include "Utilities/Geometry/Polygon.hpp"
#include "Utilities/Geometry/Rectangle.hpp"
#include "Utilities/Geometry/Transform.hpp"
#include "Utilities/Geometry/ValueEase.hpp"
#include "Utilities/Geometry/Vector2.hpp"
#include "Utilities/Geometry/Vector3.hpp"
#include "Utilities/Time/DateTime.hpp"
#include "Utilities/Time/TimeInterval.hpp"

#include "Window/AssetManager.hpp"
#include "Window/Window.hpp"
#include "Window/Viewport.hpp"
#include "Window/MessageBox.hpp"

#include "World/World.hpp"
#include "World/WorldCamera.hpp"
#include "World/WorldObject.hpp"

#include "World/Aspects/Drawing/AnimationControllerAspect.hpp"
#include "World/Aspects/Drawing/ColorBoxAspect.hpp"
#include "World/Aspects/Drawing/Drawable2DAspect.hpp"
#include "World/Aspects/Drawing/SpriteAspect.hpp"

#include "World/Aspects/Movement/Direction2DAspect.hpp"
#include "World/Aspects/Movement/Transform2DAspect.hpp"
#include "World/Aspects/Movement/Transform3DAspect.hpp"
#include "World/Aspects/Movement/Velocity2DAspect.hpp"

#include "World/Aspects/Physics/Collidable2DAspect.hpp"
#include "World/Aspects/Physics/BoxCollidable2DAspect.hpp"
#include "World/Aspects/Physics/PolygonCollidable2DAspect.hpp"
#include "World/Aspects/Physics/Physics2DResponderAspect.hpp"

#ifndef TARGETPLATFORM_WINDOWS
	#ifndef main
		#define main GameLibrary_main
	#endif
#endif
