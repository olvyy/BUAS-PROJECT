#include "game.h"
#include "surface.h"
#include "template.h"
#include <SDL.h>
#include <iostream>
#include "player.hpp"
#include <cassert>
#include "Rectangle.hpp"
#include "tilemap.hpp"

std::shared_ptr<Player> player; //global player object
std::shared_ptr<GameMap> gameMap; //global map object

namespace Tmpl8
{
    //HANDLES THE NECESSARY SETUP BEFORE THE GAMELOOP STARTS______________________________________________________________________________
    //____________________________________________________________________________________________________________________________________
    void Game::Init()
    {
        //player sprites
        std::shared_ptr<Sprite> idle = std::make_shared<Sprite>(new Surface("assets/player_idle.png"), 4);
        std::shared_ptr<Sprite> moveUp = std::make_shared<Sprite>(new Surface("assets/player_up.png"), 4);
        std::shared_ptr<Sprite> moveDown = std::make_shared<Sprite>(new Surface("assets/player_down.png"), 4);
        std::shared_ptr<Sprite> moveLeft = std::make_shared<Sprite>(new Surface("assets/player_left.png"), 4);
        std::shared_ptr<Sprite> moveRight = std::make_shared<Sprite>(new Surface("assets/player_right.png"), 4);

        //create player

		vec2 position((screen->GetWidth() / 2), screen->GetHeight() / 2);
		vec2 playersize(40.0f, 40.0f);

        player = std::make_shared<Player>(
            position,
            playersize,
            idle,
            moveDown,
            moveUp,
            moveLeft,
            moveRight
        );

        gameMap = std::make_shared<GameMap>();
    }

    //MAIN GAME TICK FUNCTION_____________________________________________________________________________________________________________
    //____________________________________________________________________________________________________________________________________
    void Game::Tick(float deltaTime)
    {
        // Clamp to prevent frame time spikes (~30 FPS)
        deltaTime = Min(deltaTime, 33.33333333f);

        screen->Clear(0);

        //gamemap
        std::vector<Rectangle> tileHitboxes = gameMap->GetTileHitboxes(); // Get all tile hitboxes
        //std::cout << "hitboxes: " << tileHitboxes.size() << std::endl;

        gameMap->DrawMap(screen, 2.5f, deltaTime);
        gameMap->UpdateTileAnimation(deltaTime);

        //player
        player->HandleInput();
        player->Update(deltaTime, player->GetHitbox(), tileHitboxes);
        player->drawPlayer(screen, deltaTime);
    }

    void Game::Shutdown()
    {}
};