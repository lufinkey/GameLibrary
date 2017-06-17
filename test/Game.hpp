
#pragma once

#include <GameLibrary/GameLibrary.hpp>

class Game : public fgl::Application
{
public:
	Game();
	virtual ~Game();

	virtual void initialize() override;
	virtual void loadContent(fgl::AssetManager* assetManager) override;
	virtual void unloadContent(fgl::AssetManager* assetManager) override;
	virtual void update(fgl::ApplicationData appData) override;
	virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;
};
