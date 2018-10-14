
PLATFORM =
ifeq ($(OS),Windows_NT)
	PLATFORM = "windows"
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		PLATFORM = "linux"
	else ifeq ($(UNAME_S),Darwin)
		PLATFORM = "darwin"
	endif
endif

COMPILER = clang

CXXFLAGS = -std=c++14
CFLAGS = 
OBJCFLAGS = -std=c++14 -fobjc-arc

TARGET = GameLibrary

BUILD_DIR = build
BIN_DIR = bin

SRC_FILES =\
	src/GameLibrary/GameLibrary.cpp\
	src/GameLibrary/Actor/Actor.cpp\
	src/GameLibrary/Actor/SpriteActor.cpp\
	src/GameLibrary/Actor/TextActor.cpp\
	src/GameLibrary/Actor/WireframeActor.cpp\
	src/GameLibrary/Animation/Animation.cpp\
	src/GameLibrary/Animation/AnimationPlayer.cpp\
	src/GameLibrary/Animation/AnimationProvider.cpp\
	src/GameLibrary/Animation/Animator.cpp\
	src/GameLibrary/Application/Application.cpp\
	src/GameLibrary/Application/ApplicationData.cpp\
	src/GameLibrary/Application/BatchLoader.cpp\
	src/GameLibrary/Application/EventManager.cpp\
	src/GameLibrary/Audio/Music.cpp\
	src/GameLibrary/Audio/Sound.cpp\
	src/GameLibrary/Draw/Drawable.cpp\
	src/GameLibrary/Draw/DrawContext.cpp\
	src/GameLibrary/Draw/DrawManager.cpp\
	src/GameLibrary/Event/Event.cpp\
	src/GameLibrary/Exception/Exception.cpp\
	src/GameLibrary/Exception/BadCastException.cpp\
	src/GameLibrary/Exception/IllegalArgumentException.cpp\
	src/GameLibrary/Exception/IllegalReturnValueException.cpp\
	src/GameLibrary/Exception/IllegalStateException.cpp\
	src/GameLibrary/Exception/InitializeLibraryException.cpp\
	src/GameLibrary/Exception/NotImplementedException.cpp\
	src/GameLibrary/Exception/NumberFormatException.cpp\
	src/GameLibrary/Exception/OutOfBoundsException.cpp\
	src/GameLibrary/Exception/Graphics/ImageOutOfBoundsException.cpp\
	src/GameLibrary/Exception/Graphics/TextureImageCreateException.cpp\
	src/GameLibrary/Exception/Graphics/TextureImageUpdateException.cpp\
	src/GameLibrary/Exception/Graphics/UnsupportedImageFormatException.cpp\
	src/GameLibrary/Exception/Screen/ScreenNavigationException.cpp\
	src/GameLibrary/Exception/Utilities/ArrayListOutOfBoundsException.cpp\
	src/GameLibrary/Exception/Utilities/BadAnyCastException.cpp\
	src/GameLibrary/Exception/Utilities/BadNumberCastException.cpp\
	src/GameLibrary/Exception/Utilities/DataOutOfBoundsException.cpp\
	src/GameLibrary/Exception/Utilities/DictionaryKeyNotFoundException.cpp\
	src/GameLibrary/Exception/Utilities/IllegalNumberOperationException.cpp\
	src/GameLibrary/Exception/Utilities/StringOutOfBoundsException.cpp\
	src/GameLibrary/Exception/Utilities/Font/RenderGlyphException.cpp\
	src/GameLibrary/Graphics/Color.cpp\
	src/GameLibrary/Graphics/Graphics.cpp\
	src/GameLibrary/Graphics/Image.cpp\
	src/GameLibrary/Graphics/PixelIterator.cpp\
	src/GameLibrary/Graphics/TextureImage.cpp\
	src/GameLibrary/Input/Keyboard.cpp\
	src/GameLibrary/Input/Mouse.cpp\
	src/GameLibrary/Input/Multitouch.cpp\
	src/GameLibrary/IO/Console.cpp\
	src/GameLibrary/IO/FileTools.cpp\
	src/GameLibrary/Network/NetworkProtocol.cpp\
	src/GameLibrary/Physics/Collidable.cpp\
	src/GameLibrary/Physics/CollisionEvent.cpp\
	src/GameLibrary/Physics/CollisionManager.cpp\
	src/GameLibrary/Physics/CollisionPair.cpp\
	src/GameLibrary/Physics/CollisionRectBuilder.cpp\
	src/GameLibrary/Physics/CollisionSide.cpp\
	src/GameLibrary/Physics/ContactEvent.cpp\
	src/GameLibrary/Physics/CollisionRects/BoxCollisionRect.cpp\
	src/GameLibrary/Physics/CollisionRects/CollisionRect.cpp\
	src/GameLibrary/Physics/CollisionRects/PixelCollisionRect.cpp\
	src/GameLibrary/Physics/CollisionRects/PolygonCollisionRect.cpp\
	src/GameLibrary/Screen/AutoLayoutCalculator.cpp\
	src/GameLibrary/Screen/AutoLayoutManager.cpp\
	src/GameLibrary/Screen/Screen.cpp\
	src/GameLibrary/Screen/ScreenElement.cpp\
	src/GameLibrary/Screen/ScreenManager.cpp\
	src/GameLibrary/Screen/Transition/FadeColorTransition.cpp\
	src/GameLibrary/Screen/Transition/FadeZoomTransition.cpp\
	src/GameLibrary/Screen/Transition/PopoverTransition.cpp\
	src/GameLibrary/Screen/Transition/SlideTransition.cpp\
	src/GameLibrary/Screen/UI/ActorMenuElement.cpp\
	src/GameLibrary/Screen/UI/AnimationElement.cpp\
	src/GameLibrary/Screen/UI/ButtonElement.cpp\
	src/GameLibrary/Screen/UI/CheckboxElement.cpp\
	src/GameLibrary/Screen/UI/DraggableElement.cpp\
	src/GameLibrary/Screen/UI/GridSelectorElement.cpp\
	src/GameLibrary/Screen/UI/ImageElement.cpp\
	src/GameLibrary/Screen/UI/MenuScreen.cpp\
	src/GameLibrary/Screen/UI/PopupScreen.cpp\
	src/GameLibrary/Screen/UI/SegmentedSelectorElement.cpp\
	src/GameLibrary/Screen/UI/TextElement.cpp\
	src/GameLibrary/Screen/UI/TextInputElement.cpp\
	src/GameLibrary/Screen/UI/TouchElement.cpp\
	src/GameLibrary/Screen/UI/ZoomPanElement.cpp\
	src/GameLibrary/SDL_ext/SDL_RWops_ext.cpp\
	src/GameLibrary/Utilities/Data.cpp\
	src/GameLibrary/Utilities/Math.cpp\
	src/GameLibrary/Utilities/Number.cpp\
	src/GameLibrary/Utilities/Plist.cpp\
	src/GameLibrary/Utilities/Retainable.cpp\
	src/GameLibrary/Utilities/Thread.cpp\
	src/GameLibrary/Utilities/Tools.cpp\
	src/GameLibrary/Utilities/Direction/OctalDirection.cpp\
	src/GameLibrary/Utilities/Direction/QuadDirection.cpp\
	src/GameLibrary/Utilities/Font/Font.cpp\
	src/GameLibrary/Utilities/Font/RenderedGlyphContainer.cpp\
	src/GameLibrary/Utilities/Time/DateTime.cpp\
	src/GameLibrary/Utilities/Time/TimeInterval.cpp\
	src/GameLibrary/Window/AssetManager.cpp\
	src/GameLibrary/Window/MessageBox.cpp\
	src/GameLibrary/Window/Viewport.cpp\
	src/GameLibrary/Window/Window.cpp\
	src/GameLibrary/World/World.cpp\
	src/GameLibrary/World/WorldCamera.cpp\
	src/GameLibrary/World/WorldObject.cpp\
	src/GameLibrary/World/Aspects/Drawing/AnimationControllerAspect.cpp\
	src/GameLibrary/World/Aspects/Drawing/ColorBoxAspect.cpp\
	src/GameLibrary/World/Aspects/Drawing/Drawable2DAspect.cpp\
	src/GameLibrary/World/Aspects/Drawing/SpriteAspect.cpp\
	src/GameLibrary/World/Aspects/Movement/Direction2DAspect.cpp\
	src/GameLibrary/World/Aspects/Movement/Transform2DAspect.cpp\
	src/GameLibrary/World/Aspects/Movement/Transform3DAspect.cpp\
	src/GameLibrary/World/Aspects/Movement/Velocity2DAspect.cpp\
	src/GameLibrary/World/Aspects/Physics/BoxCollidable2DAspect.cpp\
	src/GameLibrary/World/Aspects/Physics/Collidable2DAspect.cpp\
	src/GameLibrary/World/Aspects/Physics/Physics2DResponderAspect.cpp\
	src/GameLibrary/World/Aspects/Physics/PolygonCollidable2DAspect.cpp
ifeq ($(PLATFORM),darwin)
	SRC_FILES +=\
		src/GameLibrary/GameLibrary.mm\
		src/GameLibrary/IO/FileTools.mm\
		src/GameLibrary/PlatformSpecific/iOS/iOSUtils.mm\
		src/GameLibrary/Utilities/Thread.mm\
		src/GameLibrary/Window/MessageBox.mm
endif

INCLUDES =\
	-I include\
	-I external/base64/include\
	-I external/pugixml/src\
	-I external/SDL/SDL/include\
	-I external/SDL/SDL_image\
	-I external/SDL/SDL_mixer\
	-I external/SDL/SDL_ttf\
	-I external/SDL/SDL_gfx

CPP_SRC_FILES=$(filter %.cpp, $(SRC_FILES))
C_SRC_FILES=$(filter %.c, $(SRC_FILES))
MM_SRC_FILES=$(filter %.mm, $(SRC_FILES))

CPP_OBJ_FILES = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(CPP_SRC_FILES)))
C_OBJ_FILES = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(C_SRC_FILES)))
MM_OBJ_FILES = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(MM_SRC_FILES)))

OBJ_FILES = $(CPP_OBJ_FILES) $(C_OBJ_FILES) $(MM_OBJ_FILES)
DEP_FILES = $(addprefix $(BUILD_DIR)/, $(addsuffix .d, $(SRC_FILES)))
OUTPUT_DIRS = $(addprefix $(BUILD_DIR)/, $(dir $(SRC_FILES)))

export CLANG_ENABLE_OBJC_ARC = 1



all: directories dependencies $(TARGET)

clean:
	@-rm -r "$(BUILD_DIR)" "$(BIN_DIR)"

directories:
	@-mkdir -p "$(BUILD_DIR)" "$(BIN_DIR)" $(OUTPUT_DIRS)

dependencies:
	@echo "building dependencies"
	@echo "finished building dependencies"

$(TARGET): $(OBJ_FILES)
	ar rcs $(BIN_DIR)/lib$(TARGET).a $(OBJ_FILES)

$(CPP_OBJ_FILES): $(BUILD_DIR)/%.o: %
	$(COMPILER) $(CXXFLAGS) $(INCLUDES) $< -MMD -MF $(BUILD_DIR)/$<.d -c -o $@

$(C_OBJ_FILES): $(BUILD_DIR)/%.o: %
	$(COMPILER) $(CFLAGS) $(INCLUDES) $< -MMD -MF $(BUILD_DIR)/$<.d -c -o $@

$(MM_OBJ_FILES): $(BUILD_DIR)/%.o: %
	$(COMPILER) $(OBJCFLAGS) $(INCLUDES) $< -MMD -MF $(BUILD_DIR)/$<.d -c -o $@

-include $(DEP_FILES)
