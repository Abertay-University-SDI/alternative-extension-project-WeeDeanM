#pragma once
#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "TextureManager.h"
#include "Player.h"
#include <random>
#include <iostream>

class exRunnerLevel2 : public BaseLevel
{
public:
	exRunnerLevel2(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm);

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;
	bool colliding(GameObject obj);
	void reset();

protected:
	TextureManager* textMan;
	Player p;
	std::vector<GameObject> BGs;
	std::vector<GameObject> jumpables;
	std::vector<GameObject> kickables;
	std::vector<GameObject> explosions;
	std::vector<GameObject> Spikes;
	std::vector<float> explosionTimer;



	GameObject progressLine;
	Player progressP;
	sf::CircleShape destinationPoint;

	GameObject moon;
	GameObject finishLine;

	float distance;
	float travelled = 0.f;
	float speed = 0.f;

	const float MAX_SPEED = 650;
	const float ACCELERATION = 250;

	int hits = 0;
	float time = 0.f;
	float objects = 0.f;
};

