
#include "Game.hpp"

using namespace fgl;

Screen* screen = nullptr;

Game::Game()
{
	//
}

Game::~Game()
{
	//
}

void Game::initialize()
{
	//
}

void Game::loadContent(fgl::AssetManager* assetManager)
{
	screen = new Screen(getWindow());
	screen->getElement()->setBackgroundColor(fgl::Colors::RED);
	
	auto draggable = new DraggableElement(fgl::RectangleD(20, 20, 100, 100));
	draggable->setBackgroundColor(fgl::Colors::BLUE);
	draggable->setDragFrameConstrainedToParentBounds(false);
	screen->getElement()->addChildElement(draggable);
}

void Game::unloadContent(fgl::AssetManager* assetManager)
{
	if(screen != nullptr) {
		delete screen;
		screen = nullptr;
	}
}

void Game::update(fgl::ApplicationData appData)
{
	screen->update(appData);
}

void Game::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
{
	screen->draw(appData, graphics);
}
