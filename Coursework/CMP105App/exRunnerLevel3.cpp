#include "exRunnerLevel3.h"

int getRandomInt4(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(gen);
}

exRunnerLevel3::exRunnerLevel3(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;
	textMan = tm;
	std::srand(static_cast<unsigned>(std::time(nullptr)));


	// setup BGs as ten images next to each other. base dimensions 1024x1024
	float bgScalar = hwnd->getSize().y / 1024.0f;
	for (int i = 0; i < 21; ++i)
	{
		GameObject bg;
		bg.setTexture(&textMan->getTexture("bg_Scroll"));
		bg.setSize(sf::Vector2f(1024.0f * bgScalar, 1024.0f * bgScalar));
		bg.setPosition(i * 1024 * bgScalar, 0);
		BGs.push_back(bg);
	}

	distance = 19 * BGs.back().getSize().x - 30;
	finishLine.setSize(sf::Vector2f(10, window->getSize().y * 0.2));
	finishLine.setPosition(distance, window->getSize().y * 0.5);
	finishLine.setFillColor(sf::Color::Yellow);

	float placementIndex = 0.f;
	while (placementIndex < distance)
	{
		// go forward a random distance:
		placementIndex += getRandomInt4(650, 800);
		// harder in the back half.
		if (placementIndex > distance / 2) placementIndex -= getRandomInt4(100, 250);
		if (placementIndex > distance) break;
		objects += 1;
		GameObject newObj;
		newObj.setPosition(placementIndex, window->getSize().y * 0.6);
		newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));

		// places jumpable box every 650-800 pixels
		newObj.setTexture(&textMan->getTexture("jumpable"));
		jumpables.push_back(newObj);



		// setup Player
		p.setPosition(30, window->getSize().y * 0.6);


		progressP.setSize(sf::Vector2f(window->getSize().y * 0.04, window->getSize().y * 0.04));
		progressP.setPosition(0.25 * window->getSize().x, 0.03 * window->getSize().y);
		progressLine.setSize(sf::Vector2f(window->getSize().x * 0.5, window->getSize().y * 0.01));
		progressLine.setFillColor(sf::Color::White);
		progressLine.setPosition(0.25 * window->getSize().x, 0.05 * window->getSize().y);
		destinationPoint.setRadius(10);
		destinationPoint.setPosition(0.75 * window->getSize().x + 5, 0.05 * window->getSize().y - 5);
		destinationPoint.setFillColor(sf::Color::Green);

		moon.setPosition(0.9 * window->getSize().x, 0);
		moon.setSize(sf::Vector2f(0.1 * window->getSize().x, 0.1 * window->getSize().x));
		moon.setTexture(&textMan->getTexture("lecturer"));
		moon.setTextureRect(sf::IntRect(0, 0, 24, 24));

		p.setDamaged(0.5);

	}
}

void exRunnerLevel3::handleInput(float dt)
{

	if (input->isKeyDown(sf::Keyboard::Space) && p.canJump())
	{
		p.setJumping(window->getSize().y * 0.25, 0.85);
	}

}

bool exRunnerLevel3::colliding(GameObject obj)
{
	if (obj.getGlobalBounds().intersects(p.getGlobalBounds()))
	{
		return true;
	}

	return false;
}

void exRunnerLevel3::update(float dt)
{
	// race over, set back to level select screen
	if (travelled >= distance)
	{
		gameState->setCurrentState(State::RESET_RUNNER);
		return;
	}

	time += dt;
	p.update(dt);
	progressP.update(dt);
	float progress = (travelled / distance) * window->getSize().x * 0.5;
	progressP.setPosition(0.25 * window->getSize().x + progress, 0.03 * window->getSize().y);
	travelled += dt * speed;

	// scroll stuff
	for (GameObject& bg : BGs)
	{
		bg.move(-dt * speed, 0);
	}
	for (GameObject& j : jumpables)
	{
		j.move(-dt * speed, 0);
	}
	finishLine.move(-dt * speed, 0);

	// speed increases to max. Changes animation speed.
	if (speed < MAX_SPEED)
		speed += dt * ACCELERATION;

	// check for collisions
	if (p.isDamaged())
	{
		// if you're damaged you can keep going.
		return;
	}
	// check for collisions
	for (int i = 0; i < jumpables.size(); ++i)
	{
		GameObject j = jumpables[i];
		if (colliding(j))
		{
			hits++;
			p.setDamaged(1);
			speed = 0.f;
			jumpables.erase(jumpables.begin() + i);
			audio->playSoundbyName("death");
		}
	}

}

void exRunnerLevel3::render()
{
	beginDraw();
	for (GameObject bg : BGs) window->draw(bg);
	window->draw(moon);
	for (GameObject j : jumpables) window->draw(j);
	window->draw(finishLine);
	window->draw(p);
	window->draw(progressLine);
	window->draw(destinationPoint);
	window->draw(progressP);
	window->draw(moon);
	endDraw();
}

void exRunnerLevel3::reset()
{
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	hits = 0;
	time = 0.f;
	objects = 0.f;
	travelled = 0.f;

	// setup Player
	p.setPosition(30, window->getSize().y * 0.6);
	speed = 0.f;

	BGs.clear();
	jumpables.clear();

	// setup BGs as ten images next to each other. base dimensions 1024x1024
	float bgScalar = window->getSize().y / 1024.0f;
	for (int i = 0; i < 21; ++i)
	{
		GameObject bg;
		bg.setTexture(&textMan->getTexture("bg_Scroll"));
		bg.setSize(sf::Vector2f(1024.0f * bgScalar, 1024.0f * bgScalar));
		bg.setPosition(i * 1024 * bgScalar, 0);
		BGs.push_back(bg);
	}

	float placementIndex = 0.f;
	while (placementIndex < distance)
	{
		// go forward a random distance:
		placementIndex += getRandomInt4(650, 800);
		// harder in the back half.
		if (placementIndex > distance / 2) placementIndex -= getRandomInt4(100, 250);
		if (placementIndex > distance) break;
		objects += 1;
		GameObject newObj;
		newObj.setPosition(placementIndex, window->getSize().y * 0.6);
		newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));

		// jumpable
		newObj.setTexture(&textMan->getTexture("jumpable"));
		jumpables.push_back(newObj);
	}
	finishLine.setPosition(distance, window->getSize().y * 0.5);
	p.setDamaged(0.5);
}
