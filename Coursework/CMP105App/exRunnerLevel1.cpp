#include "exRunnerLevel1.h"

int getRandomInt2(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(gen);
}

exRunnerLevel1::exRunnerLevel1(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
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

	////adds all bird flying frames
	for (int i = 6; i < 11; i++)
	{
		fly.addFrame(sf::IntRect(i * 32, 0, 32, 32));
	}
	fly.setFrameSpeed(1.f / 4.f);

	//sets animation to fly flipped
	currentAnimation = &fly;
	currentAnimation->setFlipped(true);

	float placementIndex = 0.f;
	while (placementIndex < distance)
	{
		// go forward a random distance:
		placementIndex += getRandomInt2(650, 800);
		// harder in the back half.
		if (placementIndex > distance / 2) placementIndex -= getRandomInt2(100, 250);
		if (placementIndex > distance) break;
		objects += 1;
		GameObject newObj;
		newObj.setPosition(placementIndex, window->getSize().y * 0.6);
		newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));
		// randomly put a jumpable, kickable or two-jumpables next to each other
		int seed = getRandomInt2(0, 4);
		switch (seed)
		{
		case 0:
		{
			// kickable
			newObj.setTexture(&textMan->getTexture("kickable"));
			newObj.setPosition(placementIndex, window->getSize().y * 0.5);
			newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.2));
			kickables.push_back(newObj);
			break;
		}
		case 1:
		{
			// jumpable
			newObj.setTexture(&textMan->getTexture("jumpable"));
			jumpables.push_back(newObj);
			break;
		}
		case 2:
		{
			newObj.setTexture(&textMan->getTexture("jumpable"));
			jumpables.push_back(newObj);
			GameObject secondObj = newObj;
			secondObj.setPosition(secondObj.getPosition().x + secondObj.getSize().x, secondObj.getPosition().y);
			jumpables.push_back(secondObj);
			break;
		}
		case 3:
		{
			//Spikes
			newObj.setTexture(&textMan->getTexture("Spikes"));
			newObj.setPosition(placementIndex, window->getSize().y * 0.65);
			newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.05));
			Spikes.push_back(newObj);
			break;
		}
		case 4:
			//bird
			bird.setTexture(&textMan->getTexture("bird"));
			bird.setPosition(placementIndex, window->getSize().y * 0.58);
			bird.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.05));

			bird.setTextureRect(currentAnimation -> getCurrentFrame());
			birds.push_back(bird);
			break;
		}


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

void exRunnerLevel1::handleInput(float dt)
{

	if (input->isKeyDown(sf::Keyboard::Space) && p.canJump())
	{
		p.setJumping(window->getSize().y * 0.25, 0.85);
	}
	if (input->isKeyDown(sf::Keyboard::Enter) && !p.isKicking() && !input->isKeyDown(sf::Keyboard::Down))
	{
		p.setKicking(0.5);
	}
	if (input->isKeyDown(sf::Keyboard::Down))
	{
		p.setDucking();
	}
	if (!input->isKeyDown(sf::Keyboard::Down) && !p.isKicking())
	{
		p.setWalking();
	}

}

bool exRunnerLevel1::colliding(GameObject obj)
{
	if (obj.getGlobalBounds().intersects(p.getGlobalBounds()))
	{
		return true;
	}

	return false;
}

void exRunnerLevel1::update(float dt)
{
	// race over;
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
	for (GameObject& k : kickables)
	{
		k.move(-dt * speed, 0);
	}
	for (GameObject& e : explosions)
	{
		e.move(-dt * speed, 0);
	}
	for (GameObject& s : Spikes)
	{
		s.move(-dt * speed, 0);
	}
	for (GameObject& b : birds)
	{
		b.move(-dt * speed, 0);
	}
	finishLine.move(-dt * speed, 0);

	// speed increases to max. Changes animation speed.
	if (speed < MAX_SPEED)
		speed += dt * ACCELERATION;
	if (!p.isKicking())
		p.currentAnimation->setFrameSpeed(25 / speed);

	// explosions
	for (int i = 0; i < explosions.size(); ++i)
	{
		explosionTimer[i] += dt;
		if (explosionTimer[i] > 0.05 && explosionTimer[i] < 0.1)
		{
			explosions[i].setTexture(&textMan->getTexture("explosion2"));
		}
		if (explosionTimer[i] > 0.1 && explosionTimer[i] < 0.15)
		{
			explosions[i].setTexture(&textMan->getTexture("explosion3"));
		}
		if (explosionTimer[i] > 0.15 && explosionTimer[i] < 0.2)
		{
			explosions[i].setTexture(&textMan->getTexture("explosion4"));
		}
		if (explosionTimer[i] > 0.2 && explosionTimer[i] < .25)
		{
			explosions[i].setTexture(&textMan->getTexture("explosion5"));
		}
		explosions[i].setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));
		if (explosionTimer[i] > 0.25)
		{
			explosions.erase(explosions.begin() + i);
			explosionTimer.erase(explosionTimer.begin() + i);

		}

	}

	
		//fly.animate(dt);
		//bird.setTextureRect(fly.getCurrentFrame());
		

	// check for collisions
	if (p.isDamaged())
	{
		// if you're damaged you can keep going.
		return;
	}
	for (int i = 0; i < kickables.size(); ++i)// k : kickables)
	{
		GameObject k = kickables[i];
		if (colliding(k))
		{
			if (p.isKicking())
			{
				// explode the box
				kickables.erase(kickables.begin() + i);
				audio->playSoundbyName("success");
				GameObject explosion;
				explosion.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));
				explosion.setPosition(p.getPosition());
				explosion.setTexture(&textMan->getTexture("explosion1"));
				explosions.push_back(explosion);
				explosionTimer.push_back(0.f);
			}
			else
			{
				hits++;
				p.setDamaged(1);
				speed = 0.f;
				// delete object so you don't collide with it again.
				kickables.erase(kickables.begin() + i);
				audio->playSoundbyName("death");
			}
		}
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

	//check for spike collisions
	for (int i = 0; i < Spikes.size(); ++i)
	{
		GameObject s = Spikes[i];
		if (colliding(s))
		{
			hits += 2;
			p.setDamaged(1);
			speed = 0.f;
			Spikes.erase(Spikes.begin() + i);
			audio->playSoundbyName("death");
		}
	}

	//check for bird collision, dont hit if ducking
	for (int i = 0; i < birds.size(); ++i)
	{
		GameObject b = birds[i];
		if (colliding(b) && !input->isKeyDown(sf::Keyboard::Down))
		{
			hits++;
			p.setDamaged(1);
			speed = 0.f;
			birds.erase(birds.begin() + i);
			audio->playSoundbyName("death");
		}
	}
	

	//currentAnimation->animate(dt);
	//bird.setTextureRect(currentAnimation->getCurrentFrame());
	
}

void exRunnerLevel1::render()
{
	beginDraw();
	for (GameObject bg : BGs) window->draw(bg);
	window->draw(moon);
	for (GameObject j : jumpables) window->draw(j);
	for (GameObject k : kickables) window->draw(k);
	for (GameObject s : Spikes) window->draw(s);
	for (GameObject b : birds) window->draw(b);
	window->draw(finishLine);
	window->draw(p);
	window->draw(progressLine);
	window->draw(destinationPoint);
	window->draw(progressP);
	for (GameObject e : explosions) window->draw(e);
	window->draw(moon);
	endDraw();
}

void exRunnerLevel1::reset()
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
	kickables.clear();
	jumpables.clear();
	Spikes.clear();
	birds.clear();

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
		placementIndex += getRandomInt2(650, 800);
		// harder in the back half.
		if (placementIndex > distance / 2) placementIndex -= getRandomInt2(100, 250);
		if (placementIndex > distance) break;
		objects += 1;
		GameObject newObj;
		newObj.setPosition(placementIndex, window->getSize().y * 0.6);
		newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));
		// randomly put a jumpable, kickable or two-jumpables next to each other
		int seed = getRandomInt2(0, 4);
		switch (seed)
		{
		case 0:
		{
			// kickable
			newObj.setTexture(&textMan->getTexture("kickable"));
			newObj.setPosition(placementIndex, window->getSize().y * 0.5);
			newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.2));
			kickables.push_back(newObj);
			break;
		}
		case 1:
		{
			// jumpable
			newObj.setTexture(&textMan->getTexture("jumpable"));
			jumpables.push_back(newObj);
			break;
		}
		case 2:
		{
			newObj.setTexture(&textMan->getTexture("jumpable"));
			jumpables.push_back(newObj);
			GameObject secondObj = newObj;
			secondObj.setPosition(secondObj.getPosition().x + secondObj.getSize().x, secondObj.getPosition().y);
			jumpables.push_back(secondObj);
			break;
		}
		case 3:
		{
			// spikes
			newObj.setTexture(&textMan->getTexture("Spikes"));
			newObj.setPosition(placementIndex, window->getSize().y * 0.65);
			newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.05));
			Spikes.push_back(newObj);
			break;
		}
		case 4:
			//bird
			bird.setTexture(&textMan->getTexture("bird"));
			bird.setPosition(placementIndex, window->getSize().y * 0.58);
			bird.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.05));

			birds.push_back(bird);
			break;
		}
	}
	finishLine.setPosition(distance, window->getSize().y * 0.5);
	p.setDamaged(0.5);
}
