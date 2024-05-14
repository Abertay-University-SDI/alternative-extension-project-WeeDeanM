#pragma once
#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include "TextureManager.h"

class TransitionRunner : public BaseLevel
{

public:
	TransitionRunner(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm);


	void handleInput(float dt) override;
	void update(float dt);
	void render();

private:
	TextureManager* textMan;
	sf::Font font;
	sf::Text explain;
	GameObject bg;
};


