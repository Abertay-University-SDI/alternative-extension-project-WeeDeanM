#include "LevelSelect.h"

LevelSelect::LevelSelect(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;
	textMan = tm;

	font.loadFromFile("font/montS.ttf");

	bg.setTexture(&textMan->getTexture("bg_Scroll"));
	float bgScalar = std::max(hwnd->getSize().x / 1920.f, hwnd->getSize().y / 1080.f);
	bg.setSize(sf::Vector2f(1920 * bgScalar, 1080 * bgScalar));

	//setup player to run :)
	p.setPosition(-300, window->getSize().y * 0.6);
	p.setVelocity(500.f, 0.f);

	explain.setFont(font);
	explain.setPosition(10, 10);
	explain.setCharacterSize(40);
	explain.setFillColor(sf::Color::Black);

	Level1.setRadius(100);
	Level1.setPosition(window->getSize().x * 0.2f, window->getSize().y * 0.1f);
	Level1.setFillColor(sf::Color::White);
	Level1.setOutlineColor(sf::Color::Green);
	Level1.setOutlineThickness(4.0f);

	Level2.setRadius(100);
	Level2.setPosition(window->getSize().x * 0.4f, window->getSize().y * 0.1f);
	Level2.setFillColor(sf::Color::White);
	Level2.setOutlineColor(sf::Color::Yellow);
	Level2.setOutlineThickness(4.0f);

	Level3.setRadius(100);
	Level3.setPosition(window->getSize().x * 0.6f, window->getSize().y * 0.1f);
	Level3.setFillColor(sf::Color::White);
	Level3.setOutlineColor(sf::Color::Red);
	Level3.setOutlineThickness(4.0f);

	levels1.setFont(font);
	levels1.setPosition(window->getSize().x * 0.2f + 82.f, window->getSize().y * 0.1f + 30.f);
	levels1.setCharacterSize(100);
	levels1.setFillColor(sf::Color::Green);
	
	levels2.setFont(font);
	levels2.setPosition(window->getSize().x * 0.4f + 78.f, window->getSize().y * 0.1f + 30.f);
	levels2.setCharacterSize(100);
	levels2.setFillColor(sf::Color::Yellow);

	levels3.setFont(font);
	levels3.setPosition(window->getSize().x * 0.6f + 78.f, window->getSize().y * 0.1f + 30.f);
	levels3.setCharacterSize(100);
	levels3.setFillColor(sf::Color::Red);
	
}

void LevelSelect::handleInput(float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		gameState->setCurrentState(State::PRE_FOUR);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		gameState->setCurrentState(State::PRE_FIVE);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		gameState->setCurrentState(State::PRE_SIX);
	}

}

/*void LevelSelect::handlemouseInput(float dt)
{
	if (sf::in && input->getMouseX() => Level1)
	{
		gameState->setCurrentState(State::RUNNER1);
	}

}*/

void LevelSelect::update(float dt)
{
	//if (p.getPosition().x - 1000 > window->getSize().x);
	//{
	//	p.setPosition(30, window->getSize().y * 0.6);
	//}
	p.move(p.getVelocity() * dt);
	p.update(dt);

	explain.setString("Use NUM keys to select level");
	levels1.setString("1");
	levels2.setString("2");
	levels3.setString("3");
}

void LevelSelect::render()
{
	beginDraw();
	window->draw(bg);
	window->draw(p);
	window->draw(explain);
	window->draw(Level1);
	window->draw(Level2);
	window->draw(Level3);
	window->draw(levels1);
	window->draw(levels2);
	window->draw(levels3);
	endDraw();
}

//bool LevelSelect::buttonPressed()
//{
//	if
//}
