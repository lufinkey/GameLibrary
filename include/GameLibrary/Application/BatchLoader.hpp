
#pragma once

#include <functional>
#include <GameLibrary/Types.hpp>
#include <GameLibrary/Utilities/ArrayList.hpp>
#include <GameLibrary/Utilities/Time/TimeInterval.hpp>

namespace fgl
{
	class BatchLoaderEventListener;
	
	/*! A callback that handles loading a single thing.\n
		\tFirst argument is the AssetManager of the calling BatchLoader\n
		\tSecond argument is a pointer to data specified when added to the BatchLoader\n
		\tThird argument is an error string, that can be set if an error occurs\n
		\n
		\tThis returns true on success, or false on failure*/
	typedef bool(*BatchLoaderFunction)(AssetManager*, String*);

	/*! Stores and loads a list of assets.*/
	class BatchLoader
	{
	public:
		/*! Constructs a BatchLoader with a given AssetManager.
			\param assetManager the AssetManager to use for loading the assets*/
		explicit BatchLoader(AssetManager*assetManager);
		/*! copy constructor*/
		BatchLoader(const BatchLoader&);
		/*! virtual destructor*/
		~BatchLoader();
		
		
		/*! Gets the AssetManager currently being used.
			\returns an AssetManager pointer*/
		AssetManager* getAssetManager() const;
		/*! Sets the AssetManager to use.
			\param assetManager the AssetManager pointer to use*/
		void setAssetManager(AssetManager*assetManager);
		
		
		/*! Adds an event listener, which gets calls functions when specific events occur. \see fgl::BatchLoaderEventListener
			\param eventListener the listener pointer*/
		void addEventListener(BatchLoaderEventListener*eventListener);
		/*! Removes a previously added event listener. If the event listener is not stored, the function returns.
			\param eventListener the listener pointer*/
		void removeEventListener(BatchLoaderEventListener*eventListener);
		
		
		/*! Adds a path to an image to load.
			\param path a path to an image file
			\param value the "load" value of the image, which gets added to the current load value \see fgl::BatchLoader::getLoadCurrent()*/
		void addTexture(const String&path, unsigned int value=1);
		/*! Adds a path to a font to load.
			\param path a path to a font file
			\param value the "load" value of the font, which gets added to the current load value \see fgl::BatchLoader::getLoadCurrent()*/
		void addFont(const String&path, unsigned int value=1);
		/*! Adds a callback function to load something.
			\param value the "load" value of the function, which gets added to the current load value \see fgl::BatchLoader::getLoadCurrent()
			\param func the loading function. This function should return true if successful, or return false and set the error message (second argument) if unsuccessful
			\throws fgl::IllegalArgumentException if callback is null*/
		void addFunction(unsigned int value, const std::function<bool(AssetManager* assetManager,String* error)>& func);
		
		
		/*! Gets the current load value. Each time an asset tries to load, this value is incremented by the asset's given value.
			\returns the current load value*/
		unsigned int getLoadCurrent();
		/*! Gets the total load value. This value is the sum of all the assets' given values.
			\returns the total load value*/
		unsigned int getLoadTotal();
		
		
		/*! Loads all of the queued assets.*/
		void loadAll();
		/*! Loads the next queued asset.*/
		void loadNext();
		/*! Stops the current load, if loadAll has been called.*/
		void stopLoad();
		
		
		/*! Removes all the queued assets.*/
		void clear();
		
	private:
		typedef enum
		{
			LOADTYPE_TEXTURE,
			LOADTYPE_FONT,
			LOADTYPE_FUNCTION
		} LoadType;

		typedef struct
		{
			String path;
			LoadType type;
			unsigned int value;
			std::function<bool(AssetManager*,String*)> func;
		} LoadInfo;

		AssetManager*assetManager;

		ArrayList<LoadInfo> loadlist;
		unsigned int loadindex;

		unsigned int loadcurrent;
		unsigned int loadtotal;
		
		ArrayList<BatchLoaderEventListener*> eventListeners;

		bool loading;
	};
	
	
	/*! An event listener for a BatchLoader's events.*/
	class BatchLoaderEventListener
	{
	public:
		/*! virtual destructor*/
		virtual ~BatchLoaderEventListener(){}
		/*! Called when the BatchLoader calls loadAll.
			\param batchLoader the current BatchLoader*/
		virtual void onBatchLoaderStart(BatchLoader* batchLoader){}
		/*! Called when the BatchLoader successfully loads a TextureImage
			\param batchLoader the current BatchLoader
			\param path the path to the loaded asset
			\param value the load value of the asset*/
		virtual void onBatchLoaderLoadTexture(BatchLoader* batchLoader, const String&path, unsigned int value){}
		/*! Called when the BatchLoader successfully loads a Font
			\param batchLoader the current BatchLoader
			\param path the path to the loaded asset
			\param value the load value of the asset*/
		virtual void onBatchLoaderLoadFont(BatchLoader* batchLoader, const String&path, unsigned int value){}
		/*! Called when the BatchLoader runs a load function that returns success.
			\param batchLoader the current BatchLoader
			\param func the load function that was called
			\param value the load value of the asset*/
		virtual void onBatchLoaderLoadFunction(BatchLoader* batchLoader, const std::function<bool(AssetManager*, String*)>& func, unsigned int value){}
		/*! Called when the BatchLoader fails to load a TextureImage
			\param batchLoader the current BatchLoader
			\param path the path to the loaded asset
			\param value the load value of the asset
			\param error the error message*/
		virtual void onBatchLoaderErrorTexture(BatchLoader* batchLoader, const String&path, unsigned int value, const String&error){}
		/*! Called when the BatchLoader fails to load a Font
			\param batchLoader the current BatchLoader
			\param path the path to the loaded asset
			\param value the load value of the asset
			\param error the error message*/
		virtual void onBatchLoaderErrorFont(BatchLoader* batchLoader, const String&path, unsigned int value, const String&error){}
		/*! Called when the BatchLoader runs a load function that returns failure.
			\param batchLoader the current BatchLoader
			\param func the load function that was called
			\param value the load value of the asset
			\param error the error message*/
		virtual void onBatchLoaderErrorFunction(BatchLoader* batchLoader, const std::function<bool(AssetManager*,String*)>& func, unsigned int value, const String& error){}
		/*! Called when the BatchLoader finishes running loadAll
			\param batchLoader the current BatchLoader*/
		virtual void onBatchLoaderFinish(BatchLoader* batchLoader){}
	};
}
