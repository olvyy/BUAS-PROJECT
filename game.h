#pragma once
#include "player.hpp"
#include "tilemap.hpp"
#include <memory>
#include <SDL.h>
#include <iostream>

namespace Tmpl8 {

class Surface;

class Game
{
public:

	void SetTarget(Surface* surface) { screen = surface; }
	void Init(); //initializer

	void Shutdown();
	void Tick( float deltaTime );
	
	bool Running() { return isRunning; }

	void MouseUp(int button) { /* implement if you want to detect mouse button presses */ }
	void MouseDown(int button) { /* implement if you want to detect mouse button presses */ }
	void MouseMove(int x, int y) { /* implement if you want to detect mouse movement */ }
	
	
	void Game::KeyDown(int key) {} //dont need this because im updating player movement in HandleInput
	void Game::KeyUp(int key) {}

protected:
	Surface* screen;
	bool isRunning = { true };
	std::shared_ptr<Player> player;
};

}; // namespace Tmpl8