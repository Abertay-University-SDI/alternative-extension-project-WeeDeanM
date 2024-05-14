#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include "TextureManager.h"

class Bird : public GameObject
{
public:
	Bird();

	void update(float dt);

	sf::Texture birdTexture;
	Animation fly;
	Animation* currentAnimation;
};

