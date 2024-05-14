#include "Bird.h"

Bird::Bird()
{
	//sets up bird object and file
	setPosition(sf::Vector2f(100, 100));
	setSize(sf::Vector2f(100, 100));

	birdTexture.loadFromFile("gfx/bird.png");
	setTexture(&birdTexture);

	//adds all bird flying frames
	for (int i = 6; i < 11; i++)
	{
		fly.addFrame(sf::IntRect(192, 0, 32, 32));
	}
	fly.setFrameSpeed(1.f / 4.f);

	//sets animation to fly flipped
	currentAnimation = &fly;
	currentAnimation->setFlipped(true);
}

void Bird::update(float dt)
{
	//keeps bird set to current animation
	currentAnimation->animate(dt);
	setTextureRect(currentAnimation->getCurrentFrame());
}
