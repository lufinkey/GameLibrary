
#include <GameLibrary/Window/AssetManager.hpp>
#include <GameLibrary/IO/FileTools.hpp>
#include <GameLibrary/Window/Window.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	AssetManager::AssetManager(Window& win, const String& root, const ArrayList<String>& secondaryRootList)
	{
		window = &win;
		rootdir = root;
		secondaryRoots = secondaryRootList;
	}

	AssetManager::~AssetManager()
	{
		unloadTextures();
		unloadFonts();
	}
	
	void AssetManager::setRootDirectory(const String& root)
	{
		rootdir = root;
	}
	
	const String& AssetManager::getRootDirectory() const
	{
		return rootdir;
	}

	void AssetManager::addSecondaryRoot(const String& root)
	{
		secondaryRoots.add(root);
	}

	const ArrayList<String>& AssetManager::getSecondaryRoots() const
	{
		return secondaryRoots;
	}

	void AssetManager::removeSecondaryRoot(const String& root)
	{
		size_t index = secondaryRoots.indexOf(root);
		if(index!=-1)
		{
			secondaryRoots.remove(index);
		}
	}
	
	Window* AssetManager::getWindow() const
	{
		return window;
	}

	bool AssetManager::loadTexture(const String& path, String* error)
	{
		for(auto& texturePair : textures)
		{
			if(texturePair.first.equals(path))
			{
				return true;
			}
		}

		for(auto assetManager : assetManagers)
		{
			if(assetManager->getTexture(path)!=nullptr)
			{
				return true;
			}
		}

		TextureImage* texture = new TextureImage();
		String fullpath = FileTools::combinePathStrings(rootdir, path);
		bool success = texture->loadFromFile(fullpath, *window->getGraphics(), error);
		if(!success)
		{
			for(auto& secondaryRoot : secondaryRoots)
			{
				fullpath = FileTools::combinePathStrings(secondaryRoot, path);
				success = texture->loadFromFile(fullpath, *window->getGraphics(), error);
				if(success)
				{
					break;
				}
			}
		}
		if(success)
		{
			if(error!=nullptr)
			{
				error->clear();
			}
			textures.add(std::pair<String,TextureImage*>(path, texture));
			return true;
		}
		else
		{
			delete texture;
			return false;
		}
	}

	void AssetManager::unloadTexture(const String& path)
	{
		for(size_t textures_size=textures.size(), i=0; i<textures_size; i++)
		{
			std::pair<String,TextureImage*>& pair = textures.get(i);
			if(pair.first.equals(path))
			{
				delete pair.second;
				textures.remove(i);
				return;
			}
		}
	}

	void AssetManager::unloadTextures()
	{
		for(auto& texturePair : textures)
		{
			delete texturePair.second;
		}
		textures.clear();
	}

	TextureImage* AssetManager::getTexture(const String& path) const
	{
		for(auto& texturePair : textures)
		{
			if(texturePair.first.equals(path))
			{
				return texturePair.second;
			}
		}
		for(auto assetManager : assetManagers)
		{
			TextureImage* texture = assetManager->getTexture(path);
			if(texture!=nullptr)
			{
				return texture;
			}
		}
		return nullptr;
	}
	
	void AssetManager::addTexture(const String& path, TextureImage* texture)
	{
		if(texture==nullptr)
		{
			throw IllegalArgumentException("texture", "cannot be null");
		}
		textures.add(std::pair<String, TextureImage*>(path, texture));
	}

	bool AssetManager::loadFont(const String& path, String* error)
	{
		for(auto& fontPair : fonts)
		{
			if(fontPair.first.equals(path))
			{
				return true;
			}
		}

		for(auto assetManager : assetManagers)
		{
			if(assetManager->getFont(path)!=nullptr)
			{
				return true;
			}
		}

		Font* font = new Font();
		String fullpath = FileTools::combinePathStrings(rootdir, path);
		bool success = font->loadFromFile(fullpath, 24, error);
		if(!success)
		{
			for(auto& secondaryRoot : secondaryRoots)
			{
				fullpath = FileTools::combinePathStrings(secondaryRoot, path);
				success = font->loadFromFile(fullpath, 24, error);
				if(success)
				{
					break;
				}
			}
		}
		if(success)
		{
			if(error!=nullptr)
			{
				error->clear();
			}
			fonts.add(std::pair<String,Font*>(path, font));
			return true;
		}
		else
		{
			delete font;
			return false;
		}
	}

	void AssetManager::unloadFont(const String& path)
	{
		for(size_t fonts_size=fonts.size(), i=0; i<fonts_size; i++)
		{
			std::pair<String,Font*>& pair = fonts.get(i);
			if(pair.first.equals(path))
			{
				delete pair.second;
				fonts.remove(i);
				return;
			}
		}
	}

	void AssetManager::unloadFonts()
	{
		for(auto& fontPair : fonts)
		{
			delete fontPair.second;
		}
		fonts.clear();
	}

	Font* AssetManager::getFont(const String& path) const
	{
		for(auto& fontPair : fonts)
		{
			if(fontPair.first.equals(path))
			{
				return fontPair.second;
			}
		}
		for(auto assetManager : assetManagers)
		{
			Font* font = assetManager->getFont(path);
			if(font!=nullptr)
			{
				return font;
			}
		}
		return nullptr;
	}
	
	void AssetManager::addFont(const String& path, Font* font)
	{
		if(font!=nullptr)
		{
			fonts.add(std::pair<String, Font*>(path, font));
		}
	}
	
	void AssetManager::addAssetManager(AssetManager* assetManager)
	{
		assetManagers.add(assetManager);
	}
	
	void AssetManager::removeAssetManager(AssetManager* assetManager)
	{
		size_t index = assetManagers.indexOf(assetManager);
		if(index!=-1)
		{
			assetManagers.remove(index);
		}
	}

	size_t AssetManager::getAssetCount() const
	{
		return textures.size()+fonts.size();
	}
	
	size_t AssetManager::reload(bool safely)
	{
		if(safely)
		{
			auto oldTextures = textures;
			auto oldFonts = fonts;
			textures.clear();
			fonts.clear();

			size_t successCounter = 0;
			for(auto& texturePair : textures)
			{
				if(loadTexture(texturePair.first))
				{
					successCounter++;
					delete texturePair.second;
				}
				else
				{
					textures.add(texturePair);
				}
			}
			for(auto& fontPair : fonts)
			{
				if(loadFont(fontPair.first))
				{
					successCounter++;
					delete fontPair.second;
				}
				else
				{
					fonts.add(fontPair);
				}
			}
			return successCounter;
		}
		else
		{
			ArrayList<String> texturePaths;
			texturePaths.reserve(textures.size());
			ArrayList<String> fontPaths;
			fontPaths.reserve(fonts.size());

			for(auto& texturePair : textures)
			{
				texturePaths.add(texturePair.first);
			}
			for(auto& fontPair : fonts)
			{
				fontPaths.add(fontPair.first);
			}

			unload();

			size_t successCounter = 0;
			for(auto& texturePath : texturePaths)
			{
				if(loadTexture(texturePath))
				{
					successCounter++;
				}
			}
			for(auto& fontPath : fontPaths)
			{
				if(loadFont(fontPath))
				{
					successCounter++;
				}
			}
			return successCounter;
		}
	}
	
	void AssetManager::unload()
	{
		unloadTextures();
		unloadFonts();
	}
}
