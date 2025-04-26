#pragma once
#include "player.hpp"
#include "tilemap.hpp"
#include "enemy.hpp"
#include <memory>
#include <string>
#include "SDL.h"
#include "enemyPool.hpp"
#include "bulletPool.hpp"

namespace Tmpl8 {

	enum GameState
	{
		MENU,
		GAMEPLAY,
		GAME_OVER
	};

	enum GameOverOptions
	{
		TRYAGAIN,
		QUIT
	};

class Surface;

class Game
{
public:

	void SetTarget(Surface* surface) { screen = surface; }
	void Init(); //initializer

	void Shutdown(float deltaTime);
	void Tick( float deltaTime );
	void UpdateMenu(float deltaTime);
	void UpdateGame(float deltaTime);
	bool shouldQuit = false;
	void ResetGame();


	bool IsReturnPressed() 
	{
		// get the current keyboard state
		const Uint8* keystates = SDL_GetKeyboardState(nullptr);
		if (currentState == MENU && keystates[SDL_SCANCODE_RETURN])
		{
			currentState = GAMEPLAY;
			return true;
		}

		if (currentState == GAME_OVER)
		{
			if (currentOption == TRYAGAIN && keystates[SDL_SCANCODE_RETURN])
			{
				ResetGame();
				currentState = GAMEPLAY;
				return true;
			}
			else if (currentOption == QUIT && keystates[SDL_SCANCODE_RETURN])
			{
				shouldQuit = true;
				return true;
			}
		}

		return false;
	}
	

	void MouseUp(int button) { /* implement if you want to detect mouse button presses */ }
	void MouseDown(int button) { /* implement if you want to detect mouse button presses */ }
	void MouseMove(int x, int y) { /* implement if you want to detect mouse movement */ }
	
	
	void Game::KeyDown(int key) {} //dont need this because im updating player movement in HandleInput
	void Game::KeyUp(int key) {}

	void HandleInput(float deltaTime);

protected:
	//game state
    GameState currentState = { MENU };   
	GameOverOptions currentOption = { TRYAGAIN };
	Surface* menu;
	Surface* gameOver;
	Surface* tryAgainLight;
	Surface* tryAgainDark;
	Surface* quitLight;
	Surface* quitDark;

	//game
	Surface* screen;
	int scaleFactor = 5;
	int screenCenterX = 400;
	int screenCenterY = 400;
	void UpdateScoreDisplay(Surface* screen);
	void ShowFinalScore(Surface* screen);

	//player
	std::shared_ptr<Player> player;
	Tmpl8::vec2 direction = { 0,0 };
	std::shared_ptr<Tmpl8::Sprite> heartSprite;
	std::shared_ptr<Tmpl8::Sprite> lastTry;
	
	//bullets
	float bulletTimer = 0.0f;
	const float bulletCooldown = 0.2f;
	std::shared_ptr<Sprite> bulletSprite = std::make_shared<Sprite>(new Surface("assets/bullet.png"), 1);
	std::vector <std::shared_ptr<bullet>> activeBullets;
	BulletPool bullets = BulletPool(20, bulletSprite);

	//tilemap
	std::vector<Rectangle> tileHitboxes; 
	std::vector<Rectangle> tiles6;

	//enemies
	std::vector<std::shared_ptr<Entity>> activeEntities;
	std::vector<std::shared_ptr<Entity>> activeEnemies;
	std::vector<Rectangle> enemyHitboxes;
	objectPool enemypool = { 3 };
	const float spawnDelay = 1.0f;
	float spawnTimer = 0.0f;

	std::vector<Tmpl8::vec2> spawnTiles =
	{
		{680, 360},
		{80, 400},
		{680, 400},
		{80, 440},
		{360, 680},
		{400, 680},
		{440, 680}, 
		{360, 80}, 
		{400, 80},
		{440, 80}, 
		{680, 320}, 
		{80, 360}
	};	

	//score

};

}; // namespace Tmpl8