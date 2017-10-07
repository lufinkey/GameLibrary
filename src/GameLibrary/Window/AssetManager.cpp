
#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <GameLibrary/Window/AssetManager.hpp>
#include <GameLibrary/IO/FileTools.hpp>
#include <GameLibrary/Window/Window.hpp>
#include <GameLibrary/Exception/IllegalArgumentException.hpp>

namespace fgl
{
	AssetManager::AssetManager(Window* window, const String& root, const ArrayList<String>& secondaryRoots)
		: window(window),
		rootdir(root),
		secondaryRoots(secondaryRoots)
	{
		if(window==nullptr)
		{
			throw fgl::IllegalArgumentException("window", "cannot be null");
		}
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

	FILE* AssetManager::openFile(const String& path, const char* mode, String* resolvedPath) const
	{
		if(FileTools::isPathAbsolute(path))
		{
			FILE* file = std::fopen(path, mode);
			if(file!=nullptr)
			{
				if(resolvedPath!=nullptr)
				{
					*resolvedPath = path;
				}
			}
			return file;
		}
		else
		{
			String fullpath = FileTools::combinePathStrings(rootdir, path);
			FILE* file = std::fopen(fullpath, mode);
			if(file==nullptr)
			{
				for(auto& secondaryRoot : secondaryRoots)
				{
					fullpath = FileTools::combinePathStrings(secondaryRoot, path);
					file = std::fopen(fullpath, mode);
					if(file!=nullptr)
					{
						break;
					}
				}
			}
			
			if(file!=nullptr)
			{
				if(resolvedPath!=nullptr)
				{
					*resolvedPath = fullpath;
				}
			}
			return file;
		}
	}

	TextureImage* AssetManager::loadTexture(const String& path, String* error)
	{
		if(auto texture = getTexture(path))
		{
			return texture;
		}
		
		FILE* file = openFile(path, "rb");
		if(file==nullptr)
		{
			if(error!=nullptr)
			{
				*error = "Unable to load file";
			}
			return nullptr;
		}
		
		TextureImage* texture = new TextureImage();
		bool success = texture->loadFromFile(file, *window->getGraphics(), error);
		std::fclose(file);
		if(success)
		{
			if(error!=nullptr)
			{
				error->clear();
			}
			textures.add(std::pair<String,TextureImage*>(path, texture));
			return texture;
		}
		else
		{
			delete texture;
			return nullptr;
		}
	}

	void AssetManager::unloadTexture(const String& path)
	{
		for(size_t textures_size=textures.size(), i=0; i<textures_size; i++)
		{
			std::pair<String,TextureImage*>& texturePair = textures.get(i);
			if(texturePair.first.equals(path))
			{
				//remove the mask first if needed
				size_t maskIndex = maskedTextures.indexWhere([&](const std::pair<TextureImage*, const Image*>& pair) -> bool {
					if(pair.first==texturePair.second)
					{
						return true;
					}
					return false;
				});
				if(maskIndex!=-1)
				{
					maskedTextures.remove(maskIndex);
				}

				//delete the texture
				delete texturePair.second;
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
		//remove all masked texture pairs since all textures are getting removed
		maskedTextures.clear();
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

	TextureImage* AssetManager::loadMaskedTexture(const fgl::String& path, const fgl::Image* imageMask, fgl::String* error)
	{
		if(auto texture = getTexture(path))
		{
			return texture;
		}

		FILE* file = openFile(path, "rb");
		if(file==nullptr)
		{
			if(error!=nullptr)
			{
				*error = "Unable to load file";
			}
			return nullptr;
		}

		//load the image first
		fgl::Image image;
		bool success = image.loadFromFile(file, error);
		std::fclose(file);
		if(!success)
		{
			return nullptr;
		}
		//mask the image with the image mask
		if(imageMask!=nullptr)
		{
			image.applyCompositeMask(*imageMask);
		}
		//load the texture from the image
		fgl::TextureImage* texture = new fgl::TextureImage();
		success = texture->loadFromImage(image, *window->getGraphics(), error);
		if(!success)
		{
			delete texture;
			return nullptr;
		}
		textures.add(std::pair<String, TextureImage*>(path, texture));

		//only add the image to maskedTextures if it exists within the asset manager
		size_t imageIndex = images.indexWhere([&](const std::pair<String, Image*>& pair) -> bool {
			if(pair.second==imageMask)
			{
				return true;
			}
			return false;
		});
		if(imageMask!=nullptr && imageIndex!=-1)
		{
			maskedTextures.add(std::pair<TextureImage*, const Image*>(texture, imageMask));
		}
		return texture;
	}

	Image* AssetManager::loadImage(const fgl::String& path, fgl::String* error)
	{
		if(auto image = getImage(path))
		{
			return image;
		}

		FILE* file = openFile(path, "rb");
		if(file==nullptr)
		{
			if(error!=nullptr)
			{
				*error = "Unable to load file";
			}
			return nullptr;
		}

		fgl::Image* image = new fgl::Image();
		bool success = image->loadFromFile(file, error);
		std::fclose(file);
		if(success)
		{
			if(error!=nullptr)
			{
				error->clear();
			}
			images.add(std::pair<fgl::String, fgl::Image*>(path, image));
			return image;
		}
		else
		{
			delete image;
			return nullptr;
		}
	}

	void AssetManager::unloadImage(const fgl::String& path)
	{
		for(size_t images_size=images.size(), i=0; i<images_size; i++)
		{
			std::pair<fgl::String, fgl::Image*>& imagePair = images.get(i);
			if(imagePair.first.equals(path))
			{
				//remove the mask first if it exists
				size_t maskIndex = maskedTextures.indexWhere([&](const std::pair<TextureImage*, const Image*>& pair) -> bool {
					if(pair.second==imagePair.second)
					{
						return true;
					}
					return false;
				});
				if(maskIndex!=-1)
				{
					maskedTextures.remove(maskIndex);
				}

				//delete the image
				delete imagePair.second;
				images.remove(i);
				return;
			}
		}
	}

	void AssetManager::unloadImages()
	{
		for(auto& imagePair : images)
		{
			delete imagePair.second;
		}
		images.clear();
		//remove all masked texture pairs since all images are getting removed
		maskedTextures.clear();
	}

	fgl::Image* AssetManager::getImage(const fgl::String& path) const
	{
		for(auto& imagePair : images)
		{
			if(imagePair.first.equals(path))
			{
				return imagePair.second;
			}
		}
		return nullptr;
	}

	void AssetManager::addImage(const fgl::String& path, fgl::Image* image)
	{
		if(image==nullptr)
		{
			throw fgl::IllegalArgumentException("image", "cannot be null");
		}
		images.add(std::pair<fgl::String, fgl::Image*>(path, image));
	}

	Font* AssetManager::loadFont(const String& path, String* error)
	{
		if(auto font = getFont(path))
		{
			return font;
		}
		
		FILE* file = openFile(path, "rb");
		if(file==nullptr)
		{
			if(error!=nullptr)
			{
				*error = "Unable to load file";
			}
			return nullptr;
		}

		Font* font = new Font();
		bool success = font->loadFromFile(file, error);
		std::fclose(file);
		if(success)
		{
			if(error!=nullptr)
			{
				error->clear();
			}
			fonts.add(std::pair<String,Font*>(path, font));
			return font;
		}
		else
		{
			delete font;
			return nullptr;
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

	const ArrayList<AssetManager*>& AssetManager::getAssetManagers() const
	{
		return assetManagers;
	}

	size_t AssetManager::getAssetCount() const
	{
		return textures.size()+images.size()+fonts.size();
	}
	
	size_t AssetManager::reload(bool safely)
	{
		//get the paths for the image masks
		struct MaskPathPair
		{
			String texturePath;
			String imagePath;
		};
		ArrayList<MaskPathPair> maskPaths;
		maskPaths.reserve(maskedTextures.size());
		for(auto& maskPair : maskedTextures)
		{
			size_t matchingTextureIndex = textures.indexWhere([&](const std::pair<String,TextureImage*>& pair) -> bool {
				if(pair.second==maskPair.first)
				{
					return true;
				}
				return false;
			});
			if(matchingTextureIndex==-1)
			{
				continue;
			}
			size_t matchingImageIndex = images.indexWhere([&](const std::pair<String, Image*>& pair) -> bool {
				if(pair.second==maskPair.second)
				{
					return true;
				}
				return false;
			});
			if(matchingImageIndex==-1)
			{
				continue;
			}
			MaskPathPair pathPair;
			pathPair.texturePath = textures[matchingTextureIndex].first;
			pathPair.imagePath = textures[matchingImageIndex].first;
			maskPaths.add(pathPair);
		}
		maskedTextures.clear();

		if(safely)
		{
			//if we're doing this the safe way, don't unload the old assets before loading the new assets

			auto oldTextures = textures;
			auto oldImages = images;
			auto oldFonts = fonts;
			
			textures.clear();
			images.clear();
			fonts.clear();

			size_t successCounter = 0;
			//load images
			for(auto& imagePair : images)
			{
				if(loadImage(imagePair.first))
				{
					successCounter++;
					delete imagePair.second;
				}
				else
				{
					images.add(imagePair);
				}
			}
			//load textures
			for(auto& texturePair : oldTextures)
			{
				size_t maskIndex = maskPaths.indexWhere([&](const MaskPathPair& pathPair) -> bool {
					if(pathPair.texturePath==texturePair.first)
					{
						return true;
					}
					return false;
				});
				if(maskIndex!=-1)
				{
					//load a masked texture
					auto& maskPathPair = maskPaths[maskIndex];
					Image* imageMask = getImage(maskPathPair.imagePath);
					if(loadMaskedTexture(texturePair.first, imageMask))
					{
						successCounter++;
						delete texturePair.second;
					}
					else
					{
						textures.add(texturePair);
					}
				}
				else
				{
					//load a normal texture
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
			}
			//load fonts
			for(auto& fontPair : oldFonts)
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
			//if we're not doing this the safe way, it doesn't matter if reloading the assets fails
			// They just won't get re-added if they fail to load

			ArrayList<String> texturePaths;
			texturePaths.reserve(textures.size());
			ArrayList<String> imagePaths;
			imagePaths.reserve(images.size());
			ArrayList<String> fontPaths;
			fontPaths.reserve(fonts.size());

			for(auto& texturePair : textures)
			{
				texturePaths.add(texturePair.first);
			}
			for(auto& imagePair : images)
			{
				imagePaths.add(imagePair.first);
			}
			for(auto& fontPair : fonts)
			{
				fontPaths.add(fontPair.first);
			}

			unloadTextures();
			unloadImages();
			unloadFonts();

			size_t successCounter = 0;
			//load images
			for(auto& imagePath : imagePaths)
			{
				if(loadImage(imagePath))
				{
					successCounter++;
				}
			}
			//load textures
			for(auto& texturePath : texturePaths)
			{
				size_t maskIndex = maskPaths.indexWhere([&](const MaskPathPair& pathPair) -> bool {
					if(pathPair.texturePath==texturePath)
					{
						return true;
					}
					return false;
				});
				if(maskIndex!=-1)
				{
					//load masked texture
					auto& maskPathPair = maskPaths[maskIndex];
					Image* imageMask = getImage(maskPathPair.imagePath);
					if(loadMaskedTexture(texturePath, imageMask))
					{
						successCounter++;
					}
				}
				else
				{
					//load normal texture
					if(loadTexture(texturePath))
					{
						successCounter++;
					}
				}
			}
			//load fonts
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
		unloadImages();
		unloadFonts();
	}

	void AssetManager::moveAssetsFrom(AssetManager& assetManager)
	{
		textures.addAll(assetManager.textures);
		assetManager.textures.clear();
		images.addAll(assetManager.images);
		assetManager.images.clear();
		maskedTextures.addAll(assetManager.maskedTextures);
		assetManager.maskedTextures.clear();
		fonts.addAll(assetManager.fonts);
		assetManager.fonts.clear();
	}
}
