#pragma once

#include "Framework/GameObject.h"
#include "Framework/Animation.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void handleInput(float dt);
	void setDamaged(float timer);
	bool isDamaged();
	void update(float dt);
	void setFlipped(bool f);
	void setJumping(float h, float t);
	bool isKicking();
	void setKicking(float t);
	void setDucking();
	void setWalking();
	bool canJump() const;

	sf::Texture dino;
	Input in;
	Animation walk;
	Animation damaged;
	Animation kick;
	Animation duck;
	Animation* currentAnimation;
	
	// jump component
	float jumpHeight;
	float jumpTimeElapsed = 0.f;
	float jumpTime = 0.f;

	// kick component
	float kickTime = 0.f;
	float kickTimeElapsed = 0.f;

	// damaged component
	float damagedTimer = 0.f;
	float damageLength = 100.0f;

	// ducking component
};

