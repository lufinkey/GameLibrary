
#include <GameLibrary/Draw/DrawManager.hpp>

namespace fgl
{
	DrawManager::DrawManager() {
		//
	}
	
	
	
	void DrawManager::addDrawable(Drawable* drawable, std::function<void(Graphics&)> filter) {
		if(drawable == nullptr) {
			throw IllegalArgumentException("drawable", "cannot be null");
		}
		drawables.push_back(DrawableNode(drawable, filter));
	}
	
	void DrawManager::removeDrawable(Drawable* drawable) {
		auto drawableIt = std::find(drawables.begin(), drawables.end(), DrawableNode(drawable));
		if(drawableIt != drawables.end()) {
			drawables.erase(drawableIt);
		}
	}
	
	void DrawManager::addDelegate(DrawManagerDelegate* delegate) {
		delegates.push_back(delegate);
	}
	
	void DrawManager::removeDelegate(DrawManagerDelegate* delegate) {
		auto delegateIt = std::find(delegates.begin(), delegates.end(), delegate);
		if(delegateIt != delegates.end()) {
			delegates.erase(delegateIt);
		}
	}
	
	void DrawManager::addListener(DrawManagerListener* listener) {
		listeners.push_back(listener);
	}
	
	void DrawManager::removeListener(DrawManagerListener* listener) {
		auto listenerIt = std::find(listeners.begin(), listeners.end(), listener);
		if(listenerIt != listeners.end()) {
			listeners.erase(listenerIt);
		}
	}
	
	
	
	void DrawManager::update(ApplicationData appData) {
		auto tmpListeners = listeners;
		
		// call listener "begin" events
		for(auto listener : tmpListeners) {
			listener->onBeginDrawManagerUpdates(this);
		}
		
		updateDrawables(appData);
		
		// call listener "end" events
		for(auto listener : tmpListeners) {
			listener->onFinishDrawManagerUpdates(this);
		}
	}
	
	void DrawManager::updateDrawables(ApplicationData appData) {
		// open for implementation
	}
	
	
	
	
	void DrawManager::draw(DrawContext context, Graphics graphics) const {
		// draw drawables
		for(auto& node : drawables) {
			if(!shouldDraw(node.drawable)) {
				continue;
			}
			if(node.filter) {
				auto nodeGraphics = graphics;
				node.filter(nodeGraphics);
				node.drawable->draw(context, nodeGraphics);
			}
			else {
				node.drawable->draw(context, graphics);
			}
		}
	}
	
	
	bool DrawManager::shouldDraw(Drawable* drawable) const {
		if(!drawable->shouldDraw(this)) {
			return false;
		}
		for(auto delegate : delegates) {
			if(!delegate->shouldDraw(this, drawable)) {
				return false;
			}
		}
		return true;
	}
	
	
	
	
	DrawManager::DrawableNode::DrawableNode(Drawable* drawable, std::function<void(Graphics&)> filter)
		: drawable(drawable),
		filter(filter) {
		//
	}
	
	bool DrawManager::DrawableNode::equals(const DrawableNode& node) const {
		if(drawable == node.drawable) {
			return true;
		}
		return false;
	}
	
	bool DrawManager::DrawableNode::operator==(const DrawableNode& node) const {
		return equals(node);
	}
	
	bool DrawManager::DrawableNode::operator !=(const DrawableNode& node) const {
		return !equals(node);
	}
	
	String DrawManager::DrawableNode::getDebugDescription() const {
		return drawable->getDebugDescription();
	}
	
	
	
	
	
	bool DrawManagerDelegate::shouldDraw(const DrawManager* drawManager, const Drawable* drawable) const {
		return true;
	}
	
	void DrawManagerListener::onBeginDrawManagerUpdates(DrawManager* drawManager) {
		// open for implementation
	}
	
	void DrawManagerListener::onFinishDrawManagerUpdates(DrawManager* drawManager) {
		// open for implementation
	}
}
