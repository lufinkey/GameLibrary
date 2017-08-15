
COMPILER = clang

CXXFLAGS = -std=c++11
CFLAGS = 
OBJCFLAGS = -fobjc-arc -std=c++11

TARGET = GameLibrary

BUILD_DIR = build
BIN_DIR = bin

SRC_FILES =\
	src/GameLibrary.cpp\
	src/Actor/Actor.cpp\
	src/Actor/Animation.cpp\
	src/Actor/AnimationPlayer.cpp\
	src/Actor/SpriteActor.cpp\
	src/Actor/TextActor.cpp\
	src/Actor/WireframeActor.cpp\
	src/Application/Application.cpp\
	src/Application/ApplicationData.cpp\
	src/Application/BatchLoader.cpp\
	src/Application/EventManager.cpp\
	src/Audio/Music.cpp\
	src/Audio/Sound.cpp\
	src/Exception/Exception.cpp\
	src/Exception/BadCastException.cpp\
	src/Exception/IllegalArgumentException.cpp\
	src/Exception/IllegalReturnValueException.cpp\
	src/Exception/IllegalStateException.cpp\
	src/Exception/InitializeLibraryException.cpp\
	src/Exception/NotImplementedException.cpp\
	src/Exception/NumberFormatException.cpp\
	src/Exception/OutOfBoundsException.cpp\
	src/Exception/Graphics/ImageOutOfBoundsException.cpp\
	src/Exception/Graphics/TextureImageCreateException.cpp\
	src/Exception/Graphics/TextureImageUpdateException.cpp\
	src/Exception/Graphics/UnsupportedImageFormatException.cpp\
	src/Exception/Screen/ScreenNavigationException.cpp\
	src/Exception/Utilities/ArrayListOutOfBoundsException.cpp\
	src/Exception/Utilities/BadAnyCastException.cpp\
	src/Exception/Utilities/BadNumberCastException.cpp\
	src/Exception/Utilities/DataOutOfBoundsException.cpp\
	src/Exception/Utilities/DictionaryKeyNotFoundException.cpp\
	src/Exception/Utilities/IllegalNumberOperationException.cpp\
	src/Exception/Utilities/StringOutOfBoundsException.cpp\
	src/Exception/Utilities/Font/RenderGlyphException.cpp\
	src/Graphics/Color.cpp\
	src/Graphics/Graphics.cpp\
	src/Graphics/Image.cpp\
	src/Graphics/PixelIterator.cpp\
	src/Graphics/TextureImage.cpp\
	src/Input/Keyboard.cpp\
	src/Input/Mouse.cpp\
	src/Input/Multitouch.cpp\
	src/IO/Console.cpp\
	src/IO/FileTools.cpp\
	src/IO/FileTools.mm\
	src/Network/NetworkProtocol.cpp\
	src/PlatformSpecific/iOS/iOSUtils.mm\
	src/Screen/AutoLayoutCalculator.cpp\
	src/Screen/AutoLayoutManager.cpp\
	src/Screen/Screen.cpp\
	src/Screen/ScreenElement.cpp\
	src/Screen/ScreenManager.cpp\
	src/Screen/Transition/FadeColorTransition.cpp\
	src/Screen/Transition/FadeZoomTransition.cpp\
	src/Screen/Transition/PopoverTransition.cpp\
	src/Screen/Transition/SlideTransition.cpp\
	src/Screen/UI/ActorMenuElement.cpp\
	src/Screen/UI/AnimationElement.cpp\
	src/Screen/UI/ButtonElement.cpp\
	src/Screen/UI/CheckboxElement.cpp\
	src/Screen/UI/ImageElement.cpp\
	src/Screen/UI/MenuScreen.cpp\
	src/Screen/UI/PopupScreen.cpp\
	src/Screen/UI/SegmentedSelectorElement.cpp\
	src/Screen/UI/TextElement.cpp\
	src/Screen/UI/TextInputElement.cpp\
	src/Screen/UI/TouchElement.cpp\
	src/Screen/UI/ZoomPanElement.cpp\
	src/Utilities/Data.cpp\
	src/Utilities/Math.cpp\
	src/Utilities/Number.cpp\
	src/Utilities/Plist.cpp\
	src/Utilities/Thread.cpp\
	src/Utilities/Thread.mm\
	src/Utilities/Tools.cpp\
	src/Utilities/Font/Font.cpp\
	src/Utilities/Font/RenderedGlyphContainer.cpp\
	src/Utilities/Time/DateTime.cpp\
	src/Utilities/Time/TimeInterval.cpp\
	src/Window/AssetManager.cpp\
	src/Window/MessageBox.cpp\
	src/Window/MessageBox.mm\
	src/Window/Viewport.cpp\
	src/Window/Window.cpp

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
