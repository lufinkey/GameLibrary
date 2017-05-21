
#pragma once

#include <utility>
#include <GameLibrary/Graphics/TextureImage.hpp>
#include <GameLibrary/Utilities/Font/Font.hpp>
#include <GameLibrary/Utilities/ArrayList.hpp>

namespace fgl
{
	class Window;
	
	/*! Manages various Application resources, such as TextureImage or Font objects.*/
	class AssetManager
	{
	public:
		/*! Constructs an AssetManager for the specified Window, in the specified root folder and secondary root folders.
			\param window the Window that the assets will be used for
			\param root the root folder to load assets
			\param secondaryRoots alternate root folders to search if an asset cannot be loaded inside the main root folder */
		explicit AssetManager(Window* window, const String& root="", const ArrayList<String>& secondaryRoots={});
		/*! virtual destructor*/
		virtual ~AssetManager();
		
		
		/*! Sets the primary root directory to load assets.
			\param root the path to the root folder to use*/
		void setRootDirectory(const String& root);
		/*! Gets the primary root directory for loading assets.
			\returns a const String reference representing the path to the root folder*/
		const String& getRootDirectory() const;
		
		
		/*! Adds a secondary directory to load from if an asset cannot load from the primary directory.
			\param root the path to the secondary directory*/
		void addSecondaryRoot(const String& root);
		/*! Gets an ArrayList<String> of the secondary directories to load from if the primary directory fails.
			\returns a const ArrayList<String> reference*/
		const ArrayList<String>& getSecondaryRoots() const;
		/*! Removes one of the secondary load directories.
			\see fgl::AssetManager::addSecondaryRoot(const String&)
			\param root the directory to remove from the list of secondary directories*/
		void removeSecondaryRoot(const String& root);
		
		
		/*! Gets the Window that assets are being loaded for.
			\returns a Window reference*/
		Window* getWindow() const;


		/*! Opens a FILE pointer.
			\param path the path to the file
			\param mode C string containing the file access mode. \see std::fopen
			\param resolvedPath the actual path of the opened file (the given path pre-pended with the asset manager root or secondary root)
			\returns a FILE pointer that must be closed using fclose, or NULL if the file could not be loaded */
		virtual FILE* openFile(const String& path, const char* mode, String* resolvedPath=nullptr) const;
		
		
		/*! Loads and stores a TextureImage from the given path.
			\param path the path to load the TextureImage, relative to the AssetManager root
			\param error a pointer to store an error string, if an error occurs
			\returns a TextureImage pointer if one was successfully loaded or was already stored in the AssetManager, or null if an error occurred*/
		TextureImage* loadTexture(const String& path, String* error=nullptr);
		/*! Unloads and deallocates a stored TextureImage. If a TextureImage with the given path is not stored, the function returns.
			\param path the path that the TextureImage was loaded from, or the path given in AssetManager::addTexture*/
		void unloadTexture(const String& path);
		/*! Unloads and deallocates all stored TextureImage objects.*/
		void unloadTextures();
		/*! Gets a stored TextureImage.
			\param path the path that the TextureImage was loaded from, or the path given in AssetManager::addTexture
			\returns a TextureImage pointer, or null if there is no TextureImage is stored with that path.*/
		TextureImage* getTexture(const String& path) const;
		/*! Adds a TextureImage to be stored and managed. Once added, the TextureImage's memory is handled by AssetManager, and will be deallocated if the TextureImage is unloaded.
			If a TextureImage is already stored with the given path, that TextureImage is deallocated and replaced with the new one.
			\throws fgl::IllegalArgumentException if the TextureImage is null
			\param path a path to identify the TextureImage
			\param texture the TextureImage pointer to add*/
		void addTexture(const String& path, TextureImage* texture);


		/*! Loads and stores a TextureImage from the given path and masks it with a specified Image.
			\param path the path to load the TextureImage from, relative to the AssetManager root
			\param imageMask the Image to use to mask the TextureImage
			\param error a pointer to store an error string, if an error occurs
			\returns a TextureImage pointer if one was successfully loaded or was already stored in the AssetManager, or null if an error occured*/
		TextureImage* loadMaskedTexture(const fgl::String& path, const fgl::Image* imageMask, fgl::String* error = nullptr);


		/*! Loads and stores an Image from the given path.
			\param path the path to load the Image from, relative to the AssetManager root
			\param error a pointer to store an error string, if an error occurs
			\returns an Image pointer if one was successfully loaded or was already stored in the AssetManager, or null if an error occurred*/
		Image* loadImage(const fgl::String& path, fgl::String* error=nullptr);
		/*! Unloads and deallocates a stored Image. If an Image with the given path is not stored, the function returns.
			\param path the path that the Image was loaded from, or the path given in AssetManager::addImage*/
		void unloadImage(const fgl::String& path);
		/*! Unloads and deallocates all stored Image objects.*/
		void unloadImages();
		/*! Gets a stored Image.
			\param path the path that the Image was loaded from, or the path given in AssetManager::addImage
			\returns an Image pointer, or null if there is no Image is stored with that path.*/
		Image* getImage(const fgl::String& path) const;
		/*! Adds an Image to be stored and managed. Once added, the Image's memory is handled by AssetManager, and will be deallocated if the Image is unloaded.
		If an Image is already stored with the given path, that Image is deallocated and replaced with the new one.
			\throws fgl::IllegalArgumentException if the Image is null
			\param path a path to identify the Image
			\param texture the Image pointer to add*/
		void addImage(const fgl::String& path, fgl::Image* image);
		
		
		/*! Loads and stores a Font from the given path.
			\param path the path to load the Font, relative to the AssetManager root
			\param error a pointer to store an error string, if an error occurs
			\returns a Font pointer if one was successfully loaded or was already stored in the AssetManager, or null if an error occurred*/
		Font* loadFont(const String& path, String* error=nullptr);
		/*! Unloads and deallocates a stored Font. If a Font with the given path is not stored, the function returns.
			\param path the path that the Font was loaded from, or the path given in AssetManager::addTexture*/
		void unloadFont(const String& path);
		/*! Unloads and deallocates all stored Font objects.*/
		void unloadFonts();
		/*! Gets a stored Font.
			\param path the path that the Font was loaded from, or the path given in AssetManager::addTexture
			\returns a Font pointer, or null if there is no Font is stored with that path.*/
		Font* getFont(const String& path) const;
		/*! Adds a Font to be stored and managed. Once added, the Font's memory is handled by AssetManager, and will be deallocated if the Font is unloaded.
			If a Font is already stored with the given path, that Font is deallocated and replaced with the new one.
			\throws fgl::IllegalArgumentException if the Font is null
			\param path a path to identify the Font
			\param font the Font pointer to add*/
		void addFont(const String& path, Font* font);
		
		
		/*! Adds a dependent asset manager to get assets from before attempting to load the asset
			\param assetManager the asset manager to add */
		void addAssetManager(AssetManager* assetManager);
		/*! Removes a dependent asset manager
			\param assetManager the asset manager to remove */
		void removeAssetManager(AssetManager* assetManager);
		/*! Gets all the dependent asset managers.
			\returns an ArrayList of asset managers*/
		const ArrayList<AssetManager*>& getAssetManagers() const;
		
		
		/*! Gives the total number of assets stored in the asset manager.
			\returns a count of the number of assets stored*/
		virtual size_t getAssetCount() const;
		/*! Reloads and re-stores all stored assets from their paths.
			\param safely fallback on old assets in the case of a reload failure
			\returns the total amount of successfully reloaded assets*/
		virtual size_t reload(bool safely=true);
		/*! Unloads and deallocates all stored assets. This does not include assets stored in dependent asset managers */
		virtual void unload();


		/*! Moves all assets from the given asset manager to the calling asset manager.
			\param assetManager the asset manager to take the assets from */
		void moveAssetsFrom(AssetManager& assetManager);
		
	private:
		Window* window;
		
		ArrayList<std::pair<String,TextureImage*>> textures;
		ArrayList<std::pair<String,Font*>> fonts;
		ArrayList<std::pair<String,Image*>> images;

		ArrayList<std::pair<TextureImage*,const Image*>> maskedTextures;
		
		String rootdir;
		ArrayList<String> secondaryRoots;
		
		ArrayList<AssetManager*> assetManagers;
	};
}
