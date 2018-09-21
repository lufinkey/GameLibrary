
#include <GameLibrary/World/WorldCamera.hpp>
#include <GameLibrary/World/World.hpp>

namespace fgl
{
	//#define DEBUG_DRAW_COLLISION_RECTS
	
	WorldCamera::WorldCamera(const RectangleD& frame, const Vector2d& center, const Vector2d& resolution)
		: center(center),
		resolution(resolution),
		screenElement(nullptr),
		world(nullptr) {
		screenElement = new WorldElement(frame, this);
	}
	
	WorldCamera::~WorldCamera() {
		if(screenElement->getParentElement() == nullptr) {
			delete screenElement;
		}
	}
	
	RectangleD WorldCamera::getFrame() const {
		return screenElement->getFrame();
	}
	
	void WorldCamera::setFrame(const RectangleD& frame) {
		screenElement->setFrame(frame);
	}
	
	Vector2d WorldCamera::getResolution() const {
		return resolution;
	}
	
	void WorldCamera::setResolution(const Vector2d& resolution_arg) {
		resolution = resolution_arg;
	}
	
	Vector2d WorldCamera::getCenter() const {
		return center;
	}
	
	void WorldCamera::setCenter(const Vector2d& center_arg) {
		center = center_arg;
	}
	
	ScreenElement* WorldCamera::getScreenElement() {
		return screenElement;
	}
	
	const ScreenElement* WorldCamera::getScreenElement() const {
		return screenElement;
	}
	
	RectangleD WorldCamera::getVisibleRect() const {
		return RectangleD(center.x - (resolution.x/2.0), center.y - (resolution.y/2.0), resolution.x, resolution.y);
	}
	
	WorldCamera::WorldElement::WorldElement(const RectangleD& frame, WorldCamera* camera)
		: ScreenElement(frame),
		camera(camera) {
		setClippedToFrame(true);
	}
	
	void WorldCamera::WorldElement::drawMain(ApplicationData appData, Graphics graphics) const {
		camera->drawWorld(camera->world, appData, graphics);
	}
	
	void WorldCamera::drawWorld(const World* world, ApplicationData appData, Graphics graphics) const {
		auto drawManager = world->getDrawManager();
		
		auto frame = getFrame();
		graphics.translate(frame.getCenter());
		graphics.scale(frame.getSize() / resolution);
		graphics.translate({ -center.x, -center.y });
		
		drawManager->draw(DrawContext(&appData, this, drawManager), graphics);
		
		// debug draw collision rects
		#ifdef DEBUG_DRAW_COLLISION_RECTS
		{
			auto debugGraphics = graphics;
			debugGraphics.setColor(Colors::RED);
			auto collisionManager = camera->world->getCollisionManager();
			for(auto collidable : collisionManager->getCollidables()) {
				for(auto collisionRect : collidable->getCollisionRects()) {
					collisionRect->draw(debugGraphics);
				}
			}
		}
		#endif
	}
}
