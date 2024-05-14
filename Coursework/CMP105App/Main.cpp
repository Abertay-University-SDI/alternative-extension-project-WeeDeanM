// Main
// Entry point of the application.
// Handles window creation and window events.
// ntains the game loop, updates input class and contains the level objects.
// @author Paul Robertson

#include <iostream>
#include "Level.h"
#include "TitleScreen.h"
#include "LevelSelect.h"
#include "TransitionLevel.h"
#include "TransitionRunner.h"
#include "RunnerLevel.h"
#include "WizardLevel.h"
#include "exRunnerLevel1.h"
#include "exRunnerLevel2.h"
#include "exRunnerLevel3.h"
#include "TextureManager.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"


void windowProcess(sf::RenderWindow* window, Input* in)
{
	// Handle window events.
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			break;
		case sf::Event::KeyPressed:
			// update input class
			in->setKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased:
			//update input class
			in->setKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			//update input class
			in->setMousePosition(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setLeftMouse(Input::MouseState::PRESSED);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setRightMouse(Input::MouseState::PRESSED);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//update input class
				in->setLeftMouse(Input::MouseState::UP);
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				in->setRightMouse(Input::MouseState::UP);
			}
			break;
		default:
			// don't handle other events
			break;
		}
	}
}

int main()
{
	//Create the window
	sf::RenderWindow window(sf::VideoMode(1600, 900), "Forbidden Rhythm");

	// Initialise input and level objects.
	AudioManager audioManager;
	Input input;
	GameState gameState;
	TextureManager* textureManager = new TextureManager();

	TitleScreen TitleScreen(&window, &input, &gameState, &audioManager, textureManager);
	LevelSelect levelSelect(&window, &input, &gameState, &audioManager, textureManager);
	Level motivationLevel(&window, &input, &gameState, &audioManager, textureManager);
	WizardLevel wizardLevel(&window, &input, &gameState, &audioManager, textureManager, 2);
	TransitionLevel transLevel(&window, &input, &gameState, &audioManager, textureManager);
	TransitionRunner transRunner(&window, &input, &gameState, &audioManager, textureManager);
	RunnerLevel runnerLevel(&window, &input, &gameState, &audioManager, textureManager);
	exRunnerLevel1 exRunnerLevel1(&window, &input, &gameState, &audioManager, textureManager);
	exRunnerLevel2 exRunnerLevel2(&window, &input, &gameState, &audioManager, textureManager);
	exRunnerLevel3 exRunnerLevel3(&window, &input, &gameState, &audioManager, textureManager);

	// begin on the title screen
	gameState.setCurrentState(State::TITLE);

	// Initialise objects for delta time
	sf::Clock clock;
	float deltaTime;

	// crank the music
	audioManager.playMusicbyName("digitalLove");


	// Game Loop
	while (window.isOpen())
	{
		//Process window events
		windowProcess(&window, &input);

		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();

		// state-dependent game loop
		switch (gameState.getCurrentState())
		{
		case State::TITLE:
			TitleScreen.handleInput(deltaTime);
			TitleScreen.update(deltaTime);
			TitleScreen.render();
			break;
		case State::SELECT:
			levelSelect.handleInput(deltaTime);
			levelSelect.update(deltaTime);
			levelSelect.render();
			break;
		case State::LEVEL:
			motivationLevel.handleInput(deltaTime);
			motivationLevel.update(deltaTime);
			motivationLevel.render();
			break;
		case State::RUNNER:
			runnerLevel.handleInput(deltaTime);
			runnerLevel.update(deltaTime);
			runnerLevel.render();
			break;
		case State::WIZARD:
			wizardLevel.handleInput(deltaTime);
			wizardLevel.update(deltaTime);
			wizardLevel.render();
			break;
		case State::RUNNER1:
			exRunnerLevel1.handleInput(deltaTime);
			exRunnerLevel1.update(deltaTime);
			exRunnerLevel1.render();
			break;
		case State::RUNNER2:
			exRunnerLevel2.handleInput(deltaTime);
			exRunnerLevel2.update(deltaTime);
			exRunnerLevel2.render();
			break;
		case State::RUNNER3:
			exRunnerLevel3.handleInput(deltaTime);
			exRunnerLevel3.update(deltaTime);
			exRunnerLevel3.render();
			break;
		case State::PRE_ONE:
		case State::PRE_TWO:
		case State::PRE_THREE:
		case State::ENDGAME:
			// All four of these states are captured in the transition level
			transLevel.handleInput(deltaTime);
			transLevel.update(deltaTime);
			transLevel.render();
			break;
		case State::PRE_FOUR:
		case State::PRE_FIVE:
		case State::PRE_SIX:
			transRunner.handleInput(deltaTime);
			transRunner.update(deltaTime);
			transRunner.render();
			break;
		case State::RESET:
			// reset levels and state.
			motivationLevel.reset();
			runnerLevel.reset();
			wizardLevel.reset();
			exRunnerLevel1.reset();
			exRunnerLevel2.reset();
			exRunnerLevel3.reset();
			// crank the music
			audioManager.playMusicbyName("digitalLove");
			gameState = GameState();
			gameState.setCurrentState(State::TITLE);
			break;
		case State::RESET_RUNNER:
			exRunnerLevel1.reset();
			exRunnerLevel2.reset();
			exRunnerLevel3.reset();
			audioManager.playMusicbyName("digitalLove");
			gameState = GameState();
			gameState.setCurrentState(State::SELECT);
			break;
		}		
		
		// Update input class, handle pressed keys
		// Must be done last.
		input.update();
	}

}
