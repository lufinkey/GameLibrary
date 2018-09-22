
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
		struct LoadInfo {
			AssetManager* assetManager;
			
			String root;
			String path;
			
			String getFullPath() const;
		};
		
		template<typename ASSET_TYPE>
		using LoaderFunc = std::function<ASSET_TYPE*(LoadInfo)>;
		template<typename ASSET_TYPE>
		using UnloaderFunc = std::function<void(ASSET_TYPE*)>;
		
		
		/*! Constructs an AssetManager for the specified Window, in the specified root folder and secondary root folders.
			\param window the Window that the assets will be used for
			\param rootdir the root folder to load assets */
		explicit AssetManager(Window* window, const String& rootdir="");
		/*! virtual destructor*/
		virtual ~AssetManager();
		
		
		/*! Sets the primary root directory to load assets.
			\param root the path to the root folder to use*/
		void setRootDirectory(const String& root);
		/*! Gets the primary root directory for loading assets.
			\returns a const String reference representing the path to the root folder*/
		const String& getRootDirectory() const;
		
		
		/*! Gets the Window that assets are being loaded for.
			\returns a Window reference*/
		Window* getWindow() const;


		/*! Opens a FILE pointer.
			\param path the path to the file
			\param mode C string containing the file access mode. \see std::fopen
			\param resolvedPath the actual path of the opened file (the given path pre-pended with the asset manager root or secondary root)
			\returns a FILE pointer that must be closed using fclose, or NULL if the file could not be loaded */
		FILE* openFile(const String& path, const char* mode, String* resolvedPath=nullptr) const;
		
		
		/*! Adds an asset type
			\param loader the function to load the asset from a given path
			\param unloader the function to unload a loaded asset*/
		template<typename ASSET_TYPE>
		void addAssetType(LoaderFunc<ASSET_TYPE> loader, UnloaderFunc<ASSET_TYPE> unloader=nullptr) {
			if(getAssetList<ASSET_TYPE>() != nullptr) {
				throw IllegalArgumentException("ASSET_TYPE", "cannot be added more than once");
			}
			assetLists.push_back(new AssetList<ASSET_TYPE>(loader, unloader));
		}
		
		/*! Checks if the asset manager has the asset type
			\returns true if the asset manager has the type*/
		template<typename ASSET_TYPE>
		bool hasAssetType() const {
			if(getAssetList<ASSET_TYPE>() != nullptr) {
				return true;
			}
			return false;
		}
		
		
		/*! Loads an asset of a given type
			\param path the path to load the asset from
			\returns the loaded asset
			\throws an error if the asset could not be loaded*/
		template<typename ASSET_TYPE>
		ASSET_TYPE* load(const String& path) {
			auto existingAsset = get<ASSET_TYPE>(path);
			if(existingAsset != nullptr) {
				return existingAsset;
			}
			auto assetList = getAssetList<ASSET_TYPE>();
			if(assetList == nullptr) {
				throw IllegalArgumentException("ASSET_TYPE", "type has not been added to the asset manager");
			}
			return assetList->load(LoadInfo{ this, rootdir, path });
		}
		
		
		/*! Unloads an asset of a given type
			\param path the path the asset was loaded from*/
		template<typename ASSET_TYPE>
		void unload(const String& path) {
			auto assetList = getAssetList<ASSET_TYPE>();
			if(assetList == nullptr) {
				throw IllegalArgumentException("ASSET_TYPE", "type has not been added to the asset manager");
			}
			assetList->unload(path);
		}
		
		/*! Unloads all assets of a given type */
		template<typename ASSET_TYPE>
		void unloadAll() {
			auto assetList = getAssetList<ASSET_TYPE>();
			if(assetList == nullptr) {
				throw IllegalArgumentException("ASSET_TYPE", "type has not been added to the asset manager");
			}
			assetList->unloadAll();
		}
		
		/*! Gets a loaded asset of a given type from this asset manager or a dependent one
			\param path the path that the asset was loaded from
			\returns the loaded asset
			\throws an error if the asset has not been loaded*/
		template<typename ASSET_TYPE>
		ASSET_TYPE* get(const String& path) {
			// attempt to load from self
			auto assetList = getAssetList<ASSET_TYPE>();
			if(assetList != nullptr) {
				auto asset = assetList->get(getValidAssetPath(path));
				if(asset != nullptr) {
					return asset;
				}
			}
			// attempt to load from dependent asset managers
			auto dependentAsset = getFromDependents<ASSET_TYPE>(path);
			if(dependentAsset != nullptr) {
				return dependentAsset;
			}
			return nullptr;
		}
		
		/*! Gets a loaded asset of a given type
			\param path the path that the asset was loaded from
			\returns the loaded asset
			\throws an error if the asset has not been loaded*/
		template<typename ASSET_TYPE>
		const ASSET_TYPE* get(const String& path) const {
			// attempt to load from self
			auto assetList = getAssetList<ASSET_TYPE>();
			if(assetList != nullptr) {
				auto asset = assetList->get(getValidAssetPath(path));
				if(asset != nullptr) {
					return asset;
				}
			}
			// attempt to load from dependent asset managers
			auto dependentAsset = getFromDependents<ASSET_TYPE>(path);
			if(dependentAsset != nullptr) {
				return dependentAsset;
			}
			return nullptr;
		}
		
		/*! Adds an asset of a given type
			\param path the path to use to reference the asset
			\param asset the asset */
		template<typename ASSET_TYPE>
		void add(const String& path, ASSET_TYPE* asset) {
			auto assetList = getAssetList<ASSET_TYPE>();
			if(assetList == nullptr) {
				throw IllegalArgumentException("ASSET_TYPE", "type has not been added to the asset manager");
			}
			return assetList->add(path, asset);
		}
		
		
		/*! Gets the full path to a loaded asset in this asset manager (not dependent asset managers)
			\param asset the asset to find the path for
			\returns the path that the asset was loaded from */
		template<typename ASSET_TYPE>
		String getPath(const ASSET_TYPE* asset) const {
			auto assetList = getAssetList<ASSET_TYPE>();
			if(assetList == nullptr) {
				throw IllegalArgumentException("ASSET_TYPE", "type has not been added to the asset manager");
			}
			return assetList->getPath(asset);
		}
		
		
		/*! Loads and stores a TextureImage from the given path.
			\param path the path to load the TextureImage, relative to the AssetManager root
			\returns a TextureImage pointer if one was successfully loaded or was already stored in the AssetManager, or null if an error occurred*/
		TextureImage* loadTexture(const String& path);
		/*! Unloads and deallocates a stored TextureImage. If a TextureImage with the given path is not stored, the function returns.
			\param path the path that the TextureImage was loaded from, or the path given in AssetManager::addTexture*/
		void unloadTexture(const String& path);
		/*! Unloads and deallocates all stored TextureImage objects.*/
		void unloadTextures();
		/*! Gets a stored TextureImage.
			\param path the path that the TextureImage was loaded from, or the path given in AssetManager::addTexture
			\returns a TextureImage pointer, or null if there is no TextureImage is stored with that path.*/
		TextureImage* getTexture(const String& path);
		/*! Gets a stored TextureImage.
			\param path the path that the TextureImage was loaded from, or the path given in AssetManager::addTexture
			\returns a TextureImage pointer, or null if there is no TextureImage is stored with that path.*/
		const TextureImage* getTexture(const String& path) const;
		/*! Adds a TextureImage to be stored and managed. Once added, the TextureImage's memory is handled by AssetManager, and will be deallocated if the TextureImage is unloaded.
			If a TextureImage is already stored with the given path, that TextureImage is deallocated and replaced with the new one.
			\throws fgl::IllegalArgumentException if the TextureImage is null
			\param path a path to identify the TextureImage
			\param texture the TextureImage pointer to add*/
		void addTexture(const String& path, TextureImage* texture);
		/*! Finds the file path of a loaded texture
			\param texture the texture to find the file path of
			\returns the path the image was loaded from
			\throws fgl::IllegalArgumentException if the texture is null or isn't in the AssetManager */
		String getTexturePath(const TextureImage* texture) const;


		/*! Loads and stores a TextureImage from the given path and masks it with a specified Image.
			\param path the path to load the TextureImage from, relative to the AssetManager root
			\param imageMask the Image to use to mask the TextureImage
			\returns a TextureImage pointer if one was successfully loaded or was already stored in the AssetManager, or null if an error occured*/
		TextureImage* loadMaskedTexture(const String& path, const Image* imageMask);




		/*! Loads and stores an Image from the given path.
			\param path the path to load the Image from, relative to the AssetManager root
			\returns an Image pointer if one was successfully loaded or was already stored in the AssetManager, or null if an error occurred*/
		Image* loadImage(const String& path);
		/*! Unloads and deallocates a stored Image. If an Image with the given path is not stored, the function returns.
			\param path the path that the Image was loaded from, or the path given in AssetManager::addImage*/
		void unloadImage(const String& path);
		/*! Unloads and deallocates all stored Image objects.*/
		void unloadImages();
		/*! Gets a stored Image.
			\param path the path that the Image was loaded from, or the path given in AssetManager::addImage
			\returns an Image pointer, or null if there is no Image is stored with that path.*/
		Image* getImage(const String& path);
		/*! Gets a stored Image.
			\param path the path that the Image was loaded from, or the path given in AssetManager::addImage
			\returns an Image pointer, or null if there is no Image is stored with that path.*/
		const Image* getImage(const String& path) const;
		/*! Adds an Image to be stored and managed. Once added, the Image's memory is handled by AssetManager, and will be deallocated if the Image is unloaded.
		If an Image is already stored with the given path, that Image is deallocated and replaced with the new one.
			\throws fgl::IllegalArgumentException if the Image is null
			\param path a path to identify the Image
			\param image the Image pointer to add*/
		void addImage(const String& path, Image* image);
		/*! Finds the path of a loaded image
			\param image the image to find the file path of
			\returns the path the image was loaded from
			\throws fgl::IllegalArgumentException if the image is null or isn't in the AssetManager */
		String getImagePath(const Image* image) const;
		
		
		
		
		/*! Loads and stores a Font from the given path.
			\param path the path to load the Font, relative to the AssetManager root
			\returns a Font pointer if one was successfully loaded or was already stored in the AssetManager, or null if an error occurred*/
		Font* loadFont(const String& path);
		/*! Unloads and deallocates a stored Font. If a Font with the given path is not stored, the function returns.
			\param path the path that the Font was loaded from, or the path given in AssetManager::addTexture*/
		void unloadFont(const String& path);
		/*! Unloads and deallocates all stored Font objects.*/
		void unloadFonts();
		/*! Gets a stored Font.
			\param path the path that the Font was loaded from, or the path given in AssetManager::addTexture
			\returns a Font pointer, or null if there is no Font is stored with that path.*/
		Font* getFont(const String& path);
		/*! Gets a stored Font.
			\param path the path that the Font was loaded from, or the path given in AssetManager::addTexture
			\returns a Font pointer, or null if there is no Font is stored with that path.*/
		const Font* getFont(const String& path) const;
		/*! Adds a Font to be stored and managed. Once added, the Font's memory is handled by AssetManager, and will be deallocated if the Font is unloaded.
			If a Font is already stored with the given path, that Font is deallocated and replaced with the new one.
			\throws fgl::IllegalArgumentException if the Font is null
			\param path a path to identify the Font
			\param font the Font pointer to add*/
		void addFont(const String& path, Font* font);
		/*! Finds the path of a loaded font
			\param font the font to find the file path of
			\returns the path the font was loaded from
			\throws fgl::IllegalArgumentException if the font is null or isn't in the AssetManager */
		String getFontPath(const Font* font) const;
		
		
		
		
		/*! Gives the total number of assets stored in the asset manager.
			\returns a count of the number of assets stored*/
		size_t getAssetCount() const;
		
		/*! Unloads and deallocates all stored assets. This does not include assets stored in dependent asset managers */
		void unloadAllAssets();
		
		
		
		/*! Adds a dependent asset manager to get assets from before attempting to load the asset
			\param assetManager the asset manager to add */
		void addAssetManager(AssetManager* assetManager);
		/*! Removes a dependent asset manager
			\param assetManager the asset manager to remove */
		void removeAssetManager(AssetManager* assetManager);
		/*! Gets all the dependent asset managers.
			\returns an ArrayList of asset managers*/
		const std::list<AssetManager*>& getAssetManagers();
		/*! Gets all the dependent asset managers.
			\returns an ArrayList of asset managers*/
		const std::list<const AssetManager*>& getAssetManagers() const;



		/*! Swaps all assets between the given asset manager and the calling asset manager.
			\param assetManager the asset manager to swap assets with */
		void swapAssets(AssetManager* assetManager);
		
	private:
		// generic "asset list" class acts as a base class for typed asset lists
		class PlainAssetList {
		public:
			virtual ~PlainAssetList() = default;
			
			virtual void unload(const String& path) = 0;
			virtual void unloadAll() = 0;
			virtual size_t getAssetCount() const = 0;
		};
		
		// manage assets for a given type
		template<typename ASSET_TYPE>
		class AssetList : public PlainAssetList {
		public:
			struct AssetNode {
				String path;
				ASSET_TYPE* asset;
			};
			
			typedef std::list<AssetNode> AssetNodeList;
			
			AssetList(LoaderFunc<ASSET_TYPE> loader, UnloaderFunc<ASSET_TYPE> unloader)
				: loader(loader), unloader(unloader) {
				//
			};
			
			virtual ~AssetList() {
				for(auto& pair : assets) {
					if(unloader) {
						unloader(pair.asset);
					}
					else {
						delete pair.asset;
					}
				}
			}
			
			typename AssetNodeList::iterator find(const String& path) {
				for(auto it=assets.begin(); it!=assets.end(); it++) {
					if(it->path == path) {
						return it;
					}
				}
				return assets.end();
			}
			
			typename AssetNodeList::const_iterator find(const String& path) const {
				for(auto it=assets.begin(); it!=assets.end(); it++) {
					if(it->path == path) {
						return it;
					}
				}
				return assets.end();
			}
			
			String getPath(const ASSET_TYPE* asset) const {
				for(auto it=assets.begin(); it!=assets.end(); it++) {
					if(it->asset == asset) {
						return it->path;
					}
				}
				throw OutOfBoundsException("asset not found");
			}
			
			
			ASSET_TYPE* get(const String& path) {
				auto assetIt = find(path);
				if(assetIt == assets.end()) {
					return nullptr;
				}
				return assetIt->asset;
			}
			
			const ASSET_TYPE* get(const String& path) const {
				auto assetIt = find(path);
				if(assetIt == assets.end()) {
					return nullptr;
				}
				return assetIt->asset;
			}
			
			ASSET_TYPE* load(LoadInfo info) {
				auto asset = loader(info);
				assets.push_back({ info.path, asset });
				return asset;
			}
			
			void add(const String& path, ASSET_TYPE* asset) {
				auto assetIt = find(path);
				if(assetIt != assets.end()) {
					throw IllegalArgumentException("path", "conflicts with already loaded asset");
				}
				assets.push_back({ path, asset });
			}
			
			virtual void unload(const String& path) override {
				auto assetIt = find(path);
				if(assetIt != assets.end()) {
					if(unloader) {
						unloader(assetIt->asset);
					}
					else {
						delete assetIt->asset;
					}
					assets.erase(assetIt);
				}
			}
			
			virtual void unloadAll() override {
				auto it = assets.begin();
				while(it != assets.end()) {
					if(unloader) {
						unloader(it->asset);
					}
					else {
						delete it->asset;
					}
					assets.erase(it);
					it = assets.begin();
				}
			}
			
			virtual size_t getAssetCount() const override {
				return assets.size();
			}
			
		private:
			AssetNodeList assets;
			LoaderFunc<ASSET_TYPE> loader;
			UnloaderFunc<ASSET_TYPE> unloader;
		};
		
		
		String getValidAssetPath(const String& path) const;
		
		
		
		// give an asset list for a specific type
		template<typename ASSET_TYPE>
		AssetList<ASSET_TYPE>* getAssetList() {
			for(auto assetList : assetLists) {
				auto typedAssetList = dynamic_cast<AssetList<ASSET_TYPE>*>(assetList);
				if(typedAssetList != nullptr) {
					return typedAssetList;
				}
			}
			return nullptr;
		}
		
		// give an asset list for a specific type
		template<typename ASSET_TYPE>
		const AssetList<ASSET_TYPE>* getAssetList() const {
			for(auto assetList : assetLists) {
				auto typedAssetList = dynamic_cast<const AssetList<ASSET_TYPE>*>(assetList);
				if(typedAssetList != nullptr) {
					return typedAssetList;
				}
			}
			return nullptr;
		}
		
		
		template<typename ASSET_TYPE>
		ASSET_TYPE* getFromDependents(const String& path) {
			for(auto assetManager : assetManagers) {
				auto asset = assetManager->get<ASSET_TYPE>(path);
				if(asset != nullptr) {
					return asset;
				}
			}
			return nullptr;
		}
		
		template<typename ASSET_TYPE>
		const ASSET_TYPE* getFromDependents(const String& path) const {
			for(auto assetManager : assetManagers) {
				auto asset = assetManager->get<ASSET_TYPE>(path);
				if(asset != nullptr) {
					return asset;
				}
			}
			return nullptr;
		}
		
		
		
		std::list<PlainAssetList*> assetLists;
		Window* window;
		String rootdir;
		std::list<AssetManager*> assetManagers;
	};
}
