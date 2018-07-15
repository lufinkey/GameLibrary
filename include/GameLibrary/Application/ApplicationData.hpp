
#pragma once

#include <GameLibrary/Utilities/Geometry/Transform.hpp>
#include <GameLibrary/Utilities/Time/TimeInterval.hpp>
#include <GameLibrary/Utilities/TypeRegistry.hpp>
#include <GameLibrary/Window/Window.hpp>

namespace fgl
{
	class Application;
	class Window;
	
	/*! Holds information about an Application, such as the Window or the AssetManager.*/
	class ApplicationData
	{
	public:
		/*! Constructs an ApplicationData object with an Application, Window, AssetManager, elapsed time, Viewport Transform, and framespeed multiplier.
			\param application the Application
			\param window the Application's Window
			\param assetManager the current AssetManager
			\param timeInterval the elapsed time of the Application
			\param transform the view transform of the Application
			\param framespeedMult the frame speed multiplier of the Application*/
		ApplicationData(Application*application, Window*window, AssetManager*assetManager, const TimeInterval&timeInterval, const TransformD&transform, double framespeedMult);


		//! Optional additional data to be passed down to draw and update functions
		Dictionary additionalData;
		
		
		/*! Gets the Application being used.
			\returns an Application pointer*/
		Application* getApplication() const;
		/*! Gets the Window being used.
			\returns a Window pointer*/
		Window* getWindow() const;
		/*! Gets the AssetManager being used.
			\returns an AssetManager pointer*/
		AssetManager* getAssetManager() const;
		/*! Gets the current elapsed time of the Application
			\returns a TimeInterval reference*/
		TimeInterval& getTime();
		/*! Gets the current elapsed time of the Application
			\returns a const TimeInterval reference*/
		const TimeInterval& getTime() const;
		/*! Gets the view transform of the Window
			\returns a Transform reference*/
		TransformD& getTransform();
		/*! Gets the Viewport Transform of the Window
			\returns a const Transform reference*/
		const TransformD& getTransform() const;
		/*! Gets the frame speed multiplier of the Application
			\returns a double value*/
		double getFrameSpeedMultiplier() const;
		
		
		/*! Sets the current Application.
			\param application the Application pointer*/
		void setApplication(Application*application);
		/*! Sets the current Window.
			\param window the Window pointer*/
		void setWindow(Window*window);
		/*! Sets the current AssetManager.
			\param assetManager the AssetManager pointer*/
		void setAssetManager(AssetManager*assetManager);
		/*! Sets the elapsed time of the Application
			\param time a constant TimeInterval reference*/
		void setTime(const TimeInterval&time);
		/*! Sets the current Viewport Transform
			\param transform a const Transform reference*/
		void setTransform(const TransformD&transform);
		
	private:
		Application* application;
		Window* window;
		AssetManager* assetManager;
		TimeInterval timeInterval;
		TransformD transform;
		double framespeedMult;
	};
}

REGISTER_TYPE(fgl::ApplicationData)
