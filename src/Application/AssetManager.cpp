
#include <GameLibrary/Application/AssetManager.hpp>
#include <GameLibrary/IO/FileTools.hpp>
#include <GameLibrary/Window/Window.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	AssetManager::AssetManager(Window&win, const String&root, const ArrayList<String>&secondaryRootList)
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
	
	void AssetManager::setRootDirectory(const String&root)
	{
		rootdir = root;
	}
	
	const String& AssetManager::getRootDirectory() const
	{
		return rootdir;
	}
	
	Window* AssetManager::getWindow() const
	{
		return window;
	}

	bool AssetManager::loadTexture(const String&path, String*error)
	{
		for(auto assetManager : assetManagers)
		{
			if(assetManager->getTexture(path)!=nullptr)
			{
				return true;
			}
		}
		
		for(unsigned int i=0; i<textures.size(); i++)
		{
			std::pair<String,TextureImage*>& pair = textures.get(i);
			if(pair.first.equals(path))
			{
				return true;
			}
		}

		TextureImage* texture = new TextureImage();
		String fullpath = FileTools::combinePathStrings(rootdir, path);
		bool success = texture->loadFromFile(fullpath, *window->getGraphics(), error);
		unsigned int secondaryIndex = 0;
		while(!success && secondaryIndex<secondaryRoots.size())
		{
			const String& secondaryRoot = secondaryRoots.get(secondaryIndex);
			fullpath = FileTools::combinePathStrings(secondaryRoot, path);
			success = texture->loadFromFile(fullpath, *window->getGraphics(), error);
			secondaryIndex++;
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

	void AssetManager::unloadTexture(const String&path)
	{
		for(unsigned int i=0; i<textures.size(); i++)
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
		for(unsigned int i=0; i<textures.size(); i++)
		{
			delete textures.get(i).second;
		}
		textures.clear();
	}

	TextureImage* AssetManager::getTexture(const String&path)
	{
		for(unsigned int i=0; i<textures.size(); i++)
		{
			std::pair<String,TextureImage*>& pair = textures.get(i);
			if(pair.first.equals(path))
			{
				return pair.second;
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
	
	void AssetManager::addTexture(const String&path, TextureImage*image)
	{
		if(image==nullptr)
		{
			throw IllegalArgumentException("image", "null");
		}
		for(size_t i=0; i<textures.size(); i++)
		{
			std::pair<String,TextureImage*>& pair = textures.get(i);
			if(pair.first.equals(path))
			{
				delete pair.second;
				textures.remove(i);
				i = textures.size();
			}
		}
		textures.add(std::pair<String, TextureImage*>(path, image));
	}

	bool AssetManager::loadFont(const String&path, String*error)
	{
		for(auto assetManager : assetManagers)
		{
			if(assetManager->getFont(path)!=nullptr)
			{
				return true;
			}
		}
		
		for(unsigned int i=0; i<fonts.size(); i++)
		{
			std::pair<String,Font*>& pair = fonts.get(i);
			if(pair.first.equals(path))
			{
				return true;
			}
		}

		Font* font = new Font();
		String fullpath = FileTools::combinePathStrings(rootdir, path);
		bool success = font->loadFromFile(fullpath, 24, error);
		unsigned int secondaryIndex = 0;
		while(!success && secondaryIndex<secondaryRoots.size())
		{
			const String& secondaryRoot = secondaryRoots.get(secondaryIndex);
			fullpath = FileTools::combinePathStrings(secondaryRoot, path);
			success = font->loadFromFile(fullpath, 24, error);
			secondaryIndex++;
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

	void AssetManager::unloadFont(const String&path)
	{
		for(unsigned int i=0; i<fonts.size(); i++)
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
		for(unsigned int i=0; i<fonts.size(); i++)
		{
			delete fonts.get(i).second;
		}
		fonts.clear();
	}

	Font* AssetManager::getFont(const String&path)
	{
		for(unsigned int i=0; i<fonts.size(); i++)
		{
			std::pair<String,Font*>& pair = fonts.get(i);
			if(pair.first.equals(path))
			{
				return pair.second;
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
	
	void AssetManager::addFont(const String&path, Font*font)
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
	
	unsigned int AssetManager::reload()
	{
		ArrayList<String> texturePaths;
		texturePaths.resize(textures.size());
		ArrayList<String> fontPaths;
		fontPaths.resize(fonts.size());
		
		for(unsigned int i=0; i<textures.size(); i++)
		{
			texturePaths.set(i, textures.get(i).first);
		}
		for(unsigned int i=0; i<fonts.size(); i++)
		{
			fontPaths.set(i, fonts.get(i).first);
		}
		
		unload();
		
		unsigned int successCounter = 0;
		for(unsigned int i=0; i<texturePaths.size(); i++)
		{
			if(loadTexture(texturePaths.get(i)))
			{
				successCounter++;
			}
		}
		for(unsigned int i=0; i<fontPaths.size(); i++)
		{
			if(loadFont(fontPaths.get(i)))
			{
				successCounter++;
			}
		}
		return successCounter;
	}
	
	void AssetManager::unload()
	{
		unloadTextures();
		unloadFonts();
	}
}
