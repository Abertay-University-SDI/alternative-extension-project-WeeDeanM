#pragma once
#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Player.h"

//This class will render the level select screen and recieve inputs that allow users to start levels.

class LevelSelect : public BaseLevel
{
public:
	//constructor
	LevelSelect(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm);

	void handleInput(float dt) override;
	//void handlemouseInput(float dt);
	void update(float dt);
	void render();
	//bool buttonPressed();

	TextureManager* textMan;
	Player p;

	//creates CircleShape objects that will be used for level buttons
	sf::CircleShape Level1;
	sf::CircleShape Level2;
	sf::CircleShape Level3;

	sf::Font font;
	sf::Text explain;
	sf::Text levels1;
	sf::Text levels2;
	sf::Text levels3;
	GameObject bg;
};

