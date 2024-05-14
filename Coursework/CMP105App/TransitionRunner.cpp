#include "TransitionRunner.h"

TransitionRunner::TransitionRunner(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
{
	// store parameters in local references.
	window = hwnd;
	input = in;
	textMan = tm;
	audio = aud;
	gameState = gs;

	// set the text object
	font.loadFromFile("font/montS.ttf");
	explain.setFont(font);
	explain.setPosition(sf::Vector2f(10, 10));
	explain.setCharacterSize(40);
	explain.setFillColor(sf::Color::Red);

	// initialise background. base size: 5760, 3240
	bg.setTexture(&textMan->getTexture("bg_Scroll"));
	float bgScalar = std::max(hwnd->getSize().x / 1920.f, hwnd->getSize().y / 1080.f);
	bg.setSize(sf::Vector2f(1920 * bgScalar, 1080 * bgScalar));
}

void TransitionRunner::handleInput(float dt)
{
	if (input->isPressed(sf::Keyboard::Enter))
	{
		switch (gameState->getCurrentState())
		{
		case State::PRE_FOUR:
			gameState->setCurrentState(State::RUNNER3);
			break;
		case State::PRE_FIVE:
			gameState->setCurrentState(State::RUNNER2);
			break;
		case State::PRE_SIX:
			gameState->setCurrentState(State::RUNNER1);
			break;
		}
	}
}

void TransitionRunner::update(float dt)
{
	// set text object by state.
	switch (gameState->getCurrentState())
	{
	case State::PRE_FOUR:
		explain.setString("In this level your dinosaur only has the ability to jump\nThe only obstacle you will face will be a simple box\nPress space to jump, find your rhythm, Good Luck!");
		break;
	case State::PRE_FIVE:
		explain.setString("In this level your dinosaur can now jump AND KICK\nKicking allows you destory wooden boxes and take no damage\nPress Space to kick\nSomeone left their spikes around so make sure not to step on them!\nGood Luck!");
		break;
	case State::PRE_SIX:
		explain.setString("In this level your dinosaur can now jump, kick and DUCK\nDucking allows you to go under birds so your next obstacle is easier\nHold down the down arrow key to duck\nYou cant kick while ducking so make sure to stand back up\nBoxes can now spawn doubled up beside each other\nGood Luck!");
		break;
	}
}

void TransitionRunner::render()
{
	beginDraw();
	window->draw(bg);
	window->draw(explain);
	endDraw();
}
