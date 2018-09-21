
#pragma once

#include <GameLibrary/Application/Application.hpp>
#include <GameLibrary/Utilities/Geometry/Vector2.hpp>

namespace fgl
{
	class DrawManager;
	class WorldCamera;
	
	class DrawContext
	{
	public:
		DrawContext(const ApplicationData* appData, const WorldCamera* camera, const DrawManager* drawManager);
		
		const ApplicationData* getAppData() const;
		const WorldCamera* getCamera() const;
		const DrawManager* getDrawManager() const;
		
		void setAppData(const ApplicationData* appData);
		void setCamera(const WorldCamera* camera);
		void setDrawManager(const DrawManager* drawManager);
		
		Vector2d getViewSize() const;

	private:
		const ApplicationData* appData;
		const WorldCamera* camera;
		const DrawManager* drawManager;
	};
}
