
COMPILER = clang

CXXFLAGS = -std=c++11
CFLAGS = 
OBJCFLAGS = -fobjc-arc -std=c++11

TARGET = GameLibrary

BUILD_DIR = build
BIN_DIR = bin

SRC_FILES =\
	src/GameLibrary/GameLibrary.cpp\
	src/GameLibrary/Actor/Actor.cpp\
	src/GameLibrary/Actor/Animation.cpp\
	src/GameLibrary/Actor/AnimationPlayer.cpp\
	src/GameLibrary/Actor/SpriteActor.cpp\
	src/GameLibrary/Actor/TextActor.cpp\
	src/GameLibrary/Actor/WireframeActor.cpp\
	src/GameLibrary/Application/Application.cpp\
	src/GameLibrary/Application/ApplicationData.cpp\
	src/GameLibrary/Application/BatchLoader.cpp\
	src/GameLibrary/Application/EventManager.cpp\
	src/GameLibrary/Audio/Music.cpp\
	src/GameLibrary/Audio/Sound.cpp\
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
	src/GameLibrary/IO/FileTools.mm\
	src/GameLibrary/Network/NetworkProtocol.cpp\
	src/GameLibrary/PlatformSpecific/iOS/iOSUtils.mm\
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
	src/GameLibrary/Screen/UI/ImageElement.cpp\
	src/GameLibrary/Screen/UI/MenuScreen.cpp\
	src/GameLibrary/Screen/UI/PopupScreen.cpp\
	src/GameLibrary/Screen/UI/SegmentedSelectorElement.cpp\
	src/GameLibrary/Screen/UI/TextElement.cpp\
	src/GameLibrary/Screen/UI/TextInputElement.cpp\
	src/GameLibrary/Screen/UI/TouchElement.cpp\
	src/GameLibrary/Screen/UI/ZoomPanElement.cpp\
	src/GameLibrary/Utilities/Data.cpp\
	src/GameLibrary/Utilities/Math.cpp\
	src/GameLibrary/Utilities/Number.cpp\
	src/GameLibrary/Utilities/Plist.cpp\
	src/GameLibrary/Utilities/Thread.cpp\
	src/GameLibrary/Utilities/Thread.mm\
	src/GameLibrary/Utilities/Tools.cpp\
	src/GameLibrary/Utilities/Font/Font.cpp\
	src/GameLibrary/Utilities/Font/RenderedGlyphContainer.cpp\
	src/GameLibrary/Utilities/Time/DateTime.cpp\
	src/GameLibrary/Utilities/Time/TimeInterval.cpp\
	src/GameLibrary/Window/AssetManager.cpp\
	src/GameLibrary/Window/MessageBox.cpp\
	src/GameLibrary/Window/MessageBox.mm\
	src/GameLibrary/Window/Viewport.cpp\
	src/GameLibrary/Window/Window.cpp

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



all: directories dependencies $(TARGET)

clean:
	@-rm -r "$(BUILD_DIR)" "$(BIN_DIR)"

directories:
	@-mkdir -p "$(BUILD_DIR)" "$(BIN_DIR)" $(OUTPUT_DIRS)

dependencies:
	@echo "building dependencies"
	@echo "finished building dependencies"

$(TARGET): $(OBJ_FILES)
	libtool -static $(OBJ_FILES) -o $(BIN_DIR)/$(TARGET).a

$(CPP_OBJ_FILES): $(BUILD_DIR)/%.o: %
	$(COMPILER) $(CXXFLAGS) $(INCLUDES) $< -MMD -MF $(BUILD_DIR)/$<.d -c -o $@

$(C_OBJ_FILES): $(BUILD_DIR)/%.o: %
	$(COMPILER) $(CFLAGS) $(INCLUDES) $< -MMD -MF $(BUILD_DIR)/$<.d -c -o $@

$(MM_OBJ_FILES): $(BUILD_DIR)/%.o: %
	$(COMPILER) $(OBJCFLAGS) $(INCLUDES) $< -MMD -MF $(BUILD_DIR)/$<.d -c -o $@

-include $(DEP_FILES)
