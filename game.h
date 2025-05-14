#pragma once
#include "player.hpp"
#include "tilemap.hpp"
#include "enemy.hpp"
#include <memory>
#include <string>
#include "SDL.h"
#include "enemyPool.hpp"
#include "bulletPool.hpp"
#include "itemPool.hpp"
#include <Audio/Sound.hpp>

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
	Game();

	void SetTarget(Surface* surface) { screen = surface; }
	void Init();
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
	void HandleInput(float deltaTime);

	
	void MouseUp(int button) { /* implement if you want to detect mouse button presses */ }
	void MouseDown(int button) { /* implement if you want to detect mouse button presses */ }
	void MouseMove(int x, int y) { /* implement if you want to detect mouse movement */ }
	
	
	void Game::KeyDown(int key) {} //dont need this because im updating player movement in HandleInput
	void Game::KeyUp(int key) {}

protected:
	//GAMESTATE
    GameState currentState = { MENU };   
	GameOverOptions currentOption = { TRYAGAIN };
	Surface* menu = nullptr;
	Surface* gameOver = nullptr;
	Surface* tryAgainLight = nullptr;
	Surface* tryAgainDark = nullptr;
	Surface* quitLight = nullptr;
	Surface* quitDark = nullptr;
	bool hasPlayedDeathSound = false;
	void UpdateScoreDisplay(Surface* screen);
	void ShowFinalScore(Surface* screen);

	//GAME
	Surface* screen = nullptr;
	Surface* frame = nullptr;
	static constexpr int scaleFactor = 5;
	static constexpr int screenCenterX = 400;
	static constexpr int screenCenterY = 400;
	
	//PLAYER
	std::shared_ptr<Player> player = nullptr;
	Tmpl8::vec2 direction = { 0,0 };
	std::shared_ptr<Tmpl8::Sprite> heartSprite = nullptr;
	std::shared_ptr<Tmpl8::Sprite> lastTry = nullptr;

	//AUDIO
	Audio::Sound explosion{ "assets/explosion.wav" };
	Audio::Sound hurt{ "assets/hurt.wav" };
	Audio::Sound powerUp{ "assets/powerUp.wav" };
	Audio::Sound death{ "assets/death.wav" };
	Audio::Sound bgMusic{ "assets/bgMusic.mp3", Audio::Sound::Type::Stream };

	//ITEMS
	std::vector<std::shared_ptr<Tmpl8::Sprite>> itemSprites;
	itemPool items;
	std::vector<std::shared_ptr<Item>> activeItems;
	std::shared_ptr<Item> ItemInUse = nullptr;
	float effectTimer = 0.0f;
	const float effectDuration = 10.0f;
	void handleItems(float deltaTime);
	const float dropChance = 20.0f;
	int GenerateRandomNumber100() 
	{
		return rand() % 100 + 1;
	}
		
	//BULLETS
	void spawnBullets(float deltaTime);
	void handleBulletEvents();
	float bulletTimer = 0.0f;
	float bulletCooldown = 0.2f;
	bool allDirectionalShooting = false;
	std::shared_ptr<Sprite> bulletSprite = nullptr;
	std::vector <std::shared_ptr<bullet>> activeBullets;
	BulletPool bullets;
	std::vector<vec2> directions =
	{
		{0, -1}, {1, -1}, {1, 0}, {1, 1},
		{0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
	};

	//TILEMAP
	std::vector<Rectangle> tileHitboxes; 
	std::vector<Rectangle> tiles6;

	//ENEMIES
	std::vector<std::shared_ptr<Entity>> activeEntities;
	std::vector<std::shared_ptr<Entity>> activeEnemies;
	std::shared_ptr<Tmpl8::Sprite> deathSprite = { nullptr };
	std::vector<Rectangle> enemyHitboxes;
	objectPool enemypool;
	float spawnDelay = 2.0f;
	const float minSpawnDelay = 1.0f;
	float spawnTimer = 0.0f;
	void handleSpawnDelay(float deltaTime);
	void spawnEnemies();
	void handleEnemyEvents(float deltaTime);

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

};

}; // namespace Tmpl8