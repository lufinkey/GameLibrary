
#include <GameLibrary/Draw/DrawContext.hpp>
#include <GameLibrary/Draw/DrawManager.hpp>
#include <GameLibrary/World/WorldCamera.hpp>

namespace fgl
{
	DrawContext::DrawContext(const ApplicationData* appData, const WorldCamera* camera, const DrawManager* drawManager)
		: appData(appData), camera(camera), drawManager(drawManager) {
		//
	}
	
	const ApplicationData* DrawContext::getAppData() const {
		return appData;
	}
	
	const WorldCamera* DrawContext::getCamera() const {
		return camera;
	}
	
	const DrawManager* DrawContext::getDrawManager() const {
		return drawManager;
	}
	
	
	void DrawContext::setAppData(const ApplicationData* appData_arg) {
		appData = appData_arg;
	}
	
	void DrawContext::setCamera(const WorldCamera* camera_arg) {
		camera = camera_arg;
	}
	
	void DrawContext::setDrawManager(const DrawManager* drawManager_arg) {
		drawManager = drawManager_arg;
	}
	
	Vector2d DrawContext::getViewSize() const {
		if(camera != nullptr) {
			return camera->getResolution();
		}
		return appData->getWindow()->getViewport()->getSize();
	}
}
