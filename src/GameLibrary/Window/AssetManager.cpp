
#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <GameLibrary/Window/AssetManager.hpp>
#include <GameLibrary/IO/FileTools.hpp>
#include <GameLibrary/Window/Window.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	String AssetManager::LoadInfo::getFullPath() const {
		if(FileTools::isPathAbsolute(path)) {
			return path;
		}
		return FileTools::combinePathStrings(root, path);
	}
	
	AssetManager::AssetManager(Window* window, const String& rootdir)
		: window(window),
		rootdir(rootdir) {
		if(window==nullptr) {
			throw fgl::IllegalArgumentException("window", "cannot be null");
		}
		
		// Image
		addAssetType<Image>([=](auto info) {
			String error;
			auto image = new Image();
			bool success = image->loadFromPath(info.getFullPath(), &error);
			if(!success) {
				delete image;
				throw Exception(error);
			}
			return image;
		});
		
		// TextureImage
		addAssetType<TextureImage>([=](auto info) {
			String error;
			auto texture = new TextureImage();
			bool success = texture->loadFromPath(info.getFullPath(), *window->getGraphics(), &error);
			if(!success) {
				delete texture;
				throw Exception(error);
			}
			return texture;
		});
		
		// Font
		addAssetType<Font>([=](auto info) {
			String error;
			auto font = new Font();
			bool success = font->loadFromPath(info.getFullPath(), &error);
			if(!success) {
				delete font;
				throw Exception(error);
			}
			return font;
		});
	}

	AssetManager::~AssetManager() {
		for(auto assetList : assetLists) {
			delete assetList;
		}
	}
	
	
	
	
	String AssetManager::getValidAssetPath(const String& path) const {
		if(FileTools::isPathAbsolute(path)) {
			return path;
		}
		else {
			return FileTools::combinePathStrings(rootdir, path);
		}
	}
	
	
	
	
	void AssetManager::setRootDirectory(const String& root) {
		rootdir = root;
	}
	
	const String& AssetManager::getRootDirectory() const {
		return rootdir;
	}
	
	Window* AssetManager::getWindow() const {
		return window;
	}




	FILE* AssetManager::openFile(const String& path, const char* mode, String* resolvedPath) const {
		auto fullpath = getValidAssetPath(path);
		FILE* file = std::fopen(fullpath, mode);
		if(file!=nullptr) {
			if(resolvedPath!=nullptr) {
				*resolvedPath = fullpath;
			}
		}
		return file;
	}
	
	
	
	
	TextureImage* AssetManager::loadTexture(const String& path) {
		return load<TextureImage>(path);
	}

	void AssetManager::unloadTexture(const String& path) {
		unload<TextureImage>(path);
	}

	void AssetManager::unloadTextures() {
		unloadAll<TextureImage>();
	}
	
	TextureImage* AssetManager::getTexture(const String& path) {
		return get<TextureImage>(path);
	}

	const TextureImage* AssetManager::getTexture(const String& path) const {
		return get<TextureImage>(path);
	}
	
	void AssetManager::addTexture(const String& path, TextureImage* texture) {
		add<TextureImage>(path, texture);
	}
	
	String AssetManager::getTexturePath(const TextureImage* texture) const {
		return getPath<TextureImage>(texture);
	}




	TextureImage* AssetManager::loadMaskedTexture(const String& path, const Image* imageMask) {
		if(auto texture = getTexture(path)) {
			return texture;
		}
		// open the file
		FILE* file = openFile(path, "rb");
		if(file==nullptr) {
			throw Exception("unable to load file");
		}

		//load the image first
		Image image;
		String error;
		bool success = image.loadFromFile(file, &error);
		FileTools::closeFile(file);
		if(!success) {
			throw Exception(error);
		}
		//mask the image with the image mask
		if(imageMask!=nullptr) {
			image.applyCompositeMask(*imageMask);
		}
		//load the texture from the image
		auto texture = new TextureImage();
		success = texture->loadFromImage(image, *window->getGraphics(), &error);
		if(!success) {
			delete texture;
			throw Exception(error);
		}
		add<TextureImage>(path, texture);
		return texture;
	}




	Image* AssetManager::loadImage(const String& path) {
		return load<Image>(path);
	}

	void AssetManager::unloadImage(const String& path) {
		unload<Image>(path);
	}

	void AssetManager::unloadImages() {
		unloadAll<Image>();
	}

	Image* AssetManager::getImage(const String& path) {
		return get<Image>(path);
	}
	
	const Image* AssetManager::getImage(const String& path) const {
		return get<Image>(path);
	}

	void AssetManager::addImage(const String& path, Image* image) {
		add<Image>(path, image);
	}
	
	String AssetManager::getImagePath(const Image* image) const {
		return getPath<Image>(image);
	}




	Font* AssetManager::loadFont(const String& path) {
		return load<Font>(path);
	}

	void AssetManager::unloadFont(const String& path) {
		unload<Font>(path);
	}

	void AssetManager::unloadFonts() {
		unloadAll<Font>();
	}

	Font* AssetManager::getFont(const String& path) {
		return get<Font>(path);
	}
	
	const Font* AssetManager::getFont(const String& path) const {
		return get<Font>(path);
	}
	
	void AssetManager::addFont(const String& path, Font* font) {
		add<Font>(path, font);
	}
	
	String AssetManager::getFontPath(const Font* font) const {
		return getPath<Font>(font);
	}




	size_t AssetManager::getAssetCount() const {
		size_t assetCount = 0;
		for(auto assetList : assetLists) {
			assetCount += assetList->getAssetCount();
		}
		return assetCount;
	}
	
	void AssetManager::unloadAllAssets() {
		for(auto assetList : assetLists) {
			assetList->unloadAll();
		}
	}




	void AssetManager::addAssetManager(AssetManager* assetManager) {
		assetManagers.push_back(assetManager);
	}
	
	void AssetManager::removeAssetManager(AssetManager* assetManager) {
		auto assetMgrIt = std::find(assetManagers.begin(), assetManagers.end(), assetManager);
		if(assetMgrIt != assetManagers.end()) {
			assetManagers.erase(assetMgrIt);
		}
	}
	
	const std::list<AssetManager*>& AssetManager::getAssetManagers() {
		return assetManagers;
	}
	
	const std::list<const AssetManager*>& AssetManager::getAssetManagers() const {
		return reinterpret_cast<const std::list<const AssetManager*>&>(assetManagers);
	}




	void AssetManager::swapAssets(AssetManager* assetManager) {
		assetLists.swap(assetManager->assetLists);
	}
}
