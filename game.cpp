#include "game.h"
#include "surface.h"
#include "template.h"
#include "player.hpp"
#include "Rectangle.hpp"
#include "tilemap.hpp"
#include "enemy.hpp"
#include "item.hpp"



#include <cassert>
#include <SDL.h>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set>


//global variables
std::shared_ptr<Player> player; 
std::shared_ptr<GameMap> gameMap; 
std::shared_ptr<Enemy> enemy;

namespace Tmpl8
{
    //HANDLES THE NECESSARY SETUP BEFORE THE GAMELOOP STARTS______________________________________________________________________________
    //____________________________________________________________________________________________________________________________________
    void Game::Init()
    {
        //menu
        menu = new Surface("assets/instructions.png");
		gameOver = new Surface("assets/gameover.png");
		tryAgainLight = new Surface("assets/tryagain1.png");
		tryAgainDark = new Surface("assets/tryagain2.png");
		quitLight = new Surface("assets/quit1.png");
		quitDark = new Surface("assets/quit2.png");

        //items
        frame = new Surface("assets/frame.png");
        itemSprites.push_back(std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/coffee.png"), 1)); //0
        itemSprites.push_back(std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/fireRate.png"), 1)); //1
        itemSprites.push_back(std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/allDirectionalShooting.png"), 1)); //2
        itemSprites.push_back(std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/heartItem.png"), 1)); //3

        //player sprites
        std::shared_ptr<Sprite> idle = std::make_shared<Sprite>(new Surface("assets/player_idle.png"), 4);
        std::shared_ptr<Sprite> moveUp = std::make_shared<Sprite>(new Surface("assets/player_up.png"), 4);
        std::shared_ptr<Sprite> moveDown = std::make_shared<Sprite>(new Surface("assets/player_down.png"), 4);
        std::shared_ptr<Sprite> moveLeft = std::make_shared<Sprite>(new Surface("assets/player_left.png"), 4);
        std::shared_ptr<Sprite> moveRight = std::make_shared<Sprite>(new Surface("assets/player_right.png"), 4);
		heartSprite = std::make_shared<Sprite>(new Surface("assets/heart.png"), 1);
		lastTry = std::make_shared<Sprite>(new Surface("assets/lasttry.png"), 1);

        //create map
        gameMap = std::make_shared<GameMap>();
        gameMap->LoadMap(gameMap->lvl1);

        //create player
        vec2 position(screenCenterX, screenCenterY);

        player = std::make_shared<Player>(
            position,
            idle,
            moveDown,
            moveUp,
            moveLeft,
            moveRight
        );

        activeEntities.push_back(player);

        //seed rand
        srand(time(NULL));
    }

    //MAIN GAME TICK FUNCTION_____________________________________________________________________________________________________________
    //____________________________________________________________________________________________________________________________________
    void Game::Tick(float deltaTime)
    {
        switch (currentState)
        {
        case MENU:
            UpdateMenu(deltaTime);
            break;
        case GAMEPLAY:
            UpdateGame(deltaTime);
            break;
        case GAME_OVER:
            Shutdown(deltaTime);
            break;
        default:
            break;
        }
    }
    void Game::UpdateMenu(float deltaTime)
    {
        int newWidth = (menu->GetWidth() * scaleFactor);
        int newHeight = (menu->GetHeight() * scaleFactor);

        if (!IsReturnPressed())
        {
			menu->DrawScaledSurface(screen, screenCenterX - (newWidth/2) , screenCenterY - (newHeight/2) , newWidth, newHeight);
        }
    }

    void Game::UpdateGame(float deltaTime)
    {
        screen->Clear(0);
        deltaTime = Min(deltaTime, 33.33333333f);
        deltaTime *= 0.001f;
        bgMusic.play();

		spawnTimer += deltaTime;
		if (spawnDelay >= minSpawnDelay)
		{
			spawnDelay -= deltaTime * 0.05f;
			//std::cout << spawnDelay << std::endl;
		}

        //gamemap
        tileHitboxes = gameMap->GetTileHitboxes();
        tiles6 = gameMap->Gettiles6();
        gameMap->DrawMap(screen, 2.5f, deltaTime);
        gameMap->UpdateTileAnimation(deltaTime);
		frame->DrawScaledSurface(screen, 15, 80, frame->GetWidth() * 2.5f, frame->GetHeight() * 2.5f);

        //player
        HandleInput(deltaTime);
        player->Update(deltaTime, tileHitboxes, tiles6, activeEntities, std::static_pointer_cast<Entity>(player), screen);
        player->Draw(screen, deltaTime);
        player->drawHearts(screen, heartSprite, lastTry, deltaTime);
        UpdateScoreDisplay(screen); 
        
		//std::cout << player->getHearts() << std::endl;

        //enemies
        {
            if (spawnTimer >= spawnDelay)
            {
                spawnTimer = 0.0f;  
                Tmpl8::vec2 lastSpawnPos;

                if (!enemypool.returnVector().empty())  
                {  
                    auto enemy = enemypool.getEnemy();  
                    Tmpl8::vec2 spawnPos;

                    while ((spawnPos == lastSpawnPos))
                    {
                        spawnPos = spawnTiles[rand() % spawnTiles.size()];
                    }

                    lastSpawnPos = spawnPos;
                    enemy->setPosition(spawnPos);

                    enemy->makeActive();  
                    enemyHitboxes.push_back(enemy->GetHitbox());  
                    activeEnemies.push_back(enemy);  
                    return;  
                }
            }

            if (!activeEnemies.empty())
            {
                for (int i = 0; i < activeEnemies.size();)
                {
                    auto& enemy = activeEnemies[i];

                    if(!player->getInvincible()) enemy->Update(deltaTime, tileHitboxes, tiles6, activeEnemies, std::static_pointer_cast<Entity>(player), screen);

                    if (enemy->isDying() == true)
                    {
                        if (!enemy->hasPlayedDeathSound())
                        {
                            explosion.play();
                            enemy->setPlayedDeathSound(true);
                        }

                        enemy->SetIdle(enemy->deathSprite);
                        enemy->Draw(screen, deltaTime);
                        ++i;
                        continue;
                    }

                    if (std::static_pointer_cast<Enemy>(enemy)->isActive())
                    {
                        enemy->Draw(screen, deltaTime);
                        
						if (collide(enemy->GetHitbox(), player->GetHitbox()))
						{
							if (!player->getInvincible())
                            {
                                if (player->getHearts() > 0)
                                {
                                    player->takeDamage(1);
                                    player->decreaseScore();
                                    player->makeInvincible();
                                    hurt.play();
                                    return;
                                }
								if (player->getHearts() <= 0)
								{
									currentState = GAME_OVER;
								}
                            }
                            std::static_pointer_cast<Enemy>(enemy)->Die();
						}

                        ++i;
                    }
                    else if (enemy->readytoPool())
                    {
                        enemypool.returnEnemy(std::static_pointer_cast<Enemy>(enemy));
                        enemy->setPlayedDeathSound(false);
                        activeEnemies.erase(activeEnemies.begin() + i);
                    }
                    else
                    {
                        ++i;
                    }
                }
            }
        }

        //bullets
        {
            for (int i = 0; i < activeBullets.size(); )
            {
                auto& bullet = activeBullets[i];

                bullet->Update(deltaTime);

                if (bullet->isActive())
                {
                    //std::cout << "bullet active" << std::endl;
                    bullet->Draw(screen);
                    ++i;
                }
                else
                {
                    bullets.ReturnBullet(bullet);
                    activeBullets.erase(activeBullets.begin() + i);
                   // std::cout << "bullet deactivated" << std::endl;

                }
            }

            //item
            for (auto& bullet : activeBullets)
            {
                if (!bullet->isActive()) continue;

               for (auto& tile : tileHitboxes)
               {
				    if (collide(bullet->getHitbox(), tile))
				    {
					    bullet->Deactivate();
				    }
               }

                for (auto& enemy : activeEnemies) 
                {
                    if (!std::static_pointer_cast<Enemy>(enemy)->isActive()) continue;

                    if (collide(bullet->getHitbox(), enemy->GetHitbox()))
                    {
                        bullet->Deactivate();
                        std::static_pointer_cast<Enemy>(enemy)->Die();
                        player->increaseScore();

						if (GenerateRandomNumber100() <= dropChance)
						{
                            if(!itemSprites.empty() && activeItems.size() < 2)
                            {
                                int randomIndex;

                                if (player->getHearts() == 3)
								{
									randomIndex = rand() % (itemSprites.size() - 1);
								}
								else if (player->getHearts() < 3)
								{
									randomIndex = rand() % itemSprites.size(); 
                                }
                                
                                auto item = items.getItem();
                                item->setSprite(itemSprites[randomIndex]);
                                item->setItemType(randomIndex);
                                item->Activate();
                                item->setPosition(enemy->getPosition());
                                activeItems.push_back(item);
                            }
						}
                    }
                }
            }
        }

        for (auto& item : activeItems)
        {
			if (!activeItems.empty()) item->Draw(screen, item->getSprite());
        }

        for (int i = 0; i < activeItems.size();)
        {
            auto& item = activeItems[i];

            if (collide(player->GetHitbox(), item->getHitbox()))
            {
                if (!player->boolCollected())
                {
                    powerUp.play();
                }

                if (item->getType() == Item::Type::HEART)
                {
                    player->increaseHearts(1);
                    item->Deactivate();
                    items.returnItem(item);
                    activeItems.erase(activeItems.begin() + i);
                    continue;
                }
                else if (!player->boolCollected())
                {
                    player->setCollected(true);
                    item->setPosition(Tmpl8::vec2(22.5, 87.5));
                    Game::ItemInUse = item;
                }
                else
                {
                    screen->Centre("ITEM IN USE", 400, 3, 3, 0xFF0000);
                }
            }

            i++;
        }


        //std::cout << player->getVelocity().x << std::endl;
        //std::cout << player->getVelocity().y << std::endl;
      
    }

    void Game::ResetGame()
    {
        player->Reset();
        
        for (auto& enemy : activeEnemies)
        {
            std::static_pointer_cast<Enemy>(enemy)->deactivate();
			enemyHitboxes.clear();
			enemypool.returnEnemy(std::static_pointer_cast<Enemy>(enemy));
        }
        activeEnemies.clear();

        player->resetHearts();

		for (auto& bullet : activeBullets)
		{
            bullet->Deactivate();
			bullets.ReturnBullet(bullet);
		}
        activeBullets.clear();

		for (auto& item : activeItems)
		{
            item->Deactivate();
			items.returnItem(item);
		}
        activeItems.clear();
		ItemInUse = nullptr;
		player->setCollected(false);
        player->resetSpeed();
		allDirectionalShooting = false;
		bulletCooldown = 0.2f;
        player->setScore(0);

		bgMusic.replay();

        effectTimer = 0.0f;
        bulletTimer = 0.0f;
        spawnTimer = 0.0f;
		hasPlayedDeathSound = false;

		vec2 resetPosition(screenCenterX, screenCenterY);
        player->setPosition(resetPosition);
		spawnDelay = 1.5f;
    }

    void Game::Shutdown(float deltaTime)
	{
        screen->Clear(0);
        HandleInput(deltaTime);
        bgMusic.stop();

		if (hasPlayedDeathSound == false)
		{
			death.play();
			hasPlayedDeathSound = true;
		}

		int gameOverWidth = (gameOver->GetWidth() * scaleFactor);
		int gameOverHeight = (gameOver->GetHeight() * scaleFactor);

		int tryAgainWidth = (tryAgainDark->GetWidth() * scaleFactor);
		int tryAgainHeight = (tryAgainDark->GetHeight() * scaleFactor);

		int quitWidth = (quitLight->GetWidth() * scaleFactor);
		int quitHeight = (quitLight->GetHeight() * scaleFactor);

		gameOver->DrawScaledSurface(screen, screenCenterX - (gameOverWidth / 2), screenCenterY - (gameOverWidth / 2), gameOverWidth, gameOverHeight);
		ShowFinalScore(screen);

        if (currentOption == TRYAGAIN)
        {
			tryAgainDark->DrawScaledSurface(screen, screenCenterX - (tryAgainWidth / 2), screenCenterY + 100 - (tryAgainHeight / 2), tryAgainWidth, tryAgainHeight);
			quitLight->DrawScaledSurface(screen, screenCenterX - (quitWidth / 2), screenCenterY + 150 - (quitHeight / 2), quitWidth, quitHeight);

            IsReturnPressed();
        }
        else
        {
            tryAgainLight->DrawScaledSurface(screen, screenCenterX - (tryAgainWidth / 2), screenCenterY + 100 - (tryAgainHeight / 2), tryAgainWidth, tryAgainHeight);
            quitDark->DrawScaledSurface(screen, screenCenterX - (quitWidth / 2), screenCenterY + 150 - (quitHeight / 2), quitWidth, quitHeight);

            IsReturnPressed();
        }
    }

    /////////////////////////////////////////////////
    /////			INPUT HANDLING			////////
    ///////////////////////////////////////////////

    void Game::HandleInput(float deltaTime)
    {
        // get the current keyboard state
        const Uint8* keystates = SDL_GetKeyboardState(nullptr);
        direction = { 0,0 };

        if (currentState == GAME_OVER)
        {
            if (keystates[SDL_SCANCODE_W])
            {
				currentOption = TRYAGAIN;
            }
            else if (keystates[SDL_SCANCODE_S])
            {
                currentOption = QUIT;
            }
        }

        if (currentState == GAMEPLAY)
        {
            //moving
            player->movingUp = keystates[SDL_SCANCODE_W];

            player->movingDown = keystates[SDL_SCANCODE_S];

            player->movingLeft = keystates[SDL_SCANCODE_A];

            player->movingRight = keystates[SDL_SCANCODE_D];

            //shooting (only if not invincible) 
            if(!player->getInvincible())
            {
                if(!allDirectionalShooting)
                {
                    if (keystates[SDL_SCANCODE_UP]) direction.y = -1;
                    if (keystates[SDL_SCANCODE_DOWN]) direction.y = 1;
                    if (keystates[SDL_SCANCODE_LEFT]) direction.x = -1;
                    if (keystates[SDL_SCANCODE_RIGHT]) direction.x = 1;
                
                    if (direction.x != 0 || direction.y != 0)
                    {
                        direction = direction.normalize(direction);
                        bulletTimer += deltaTime;

                        if (bulletTimer >= bulletCooldown)
                        {
                            auto bullet = bullets.GetBullet();
                            bullet->Activate(player->GetHitbox().Center(), direction);
                            activeBullets.push_back(bullet);
                            bulletTimer = 0;
                        }
                    }
                }
                else
                {
                    for (auto& dir : directions)
                    {
                        direction = dir.normalize(dir);
                        bulletTimer += deltaTime;

                        if(bulletTimer >= bulletCooldown)
                        {
                            auto bullet = bullets.GetBullet();
                            bullet->Activate(player->GetHitbox().Center(), direction);
                            activeBullets.push_back(bullet);
                            bulletTimer = 0;
                        }
                    }
                }
            }
                
            
            if (Game::ItemInUse)
            {
                if (player->boolCollected())
                {
                    switch (ItemInUse->getType())
                    {
                    case Item::Type::SPEED_UP:
                    {
                        effectTimer += deltaTime;
                        if (effectTimer <= effectDuration)
                        {
                            std::cout << effectTimer << std::endl;
                            player->increaseSpeed();
                        }
                        else
                        {
                            ItemInUse->Deactivate();
                            items.returnItem(ItemInUse);  
							activeItems.erase(std::remove(activeItems.begin(), activeItems.end(), ItemInUse), activeItems.end());
                            ItemInUse = nullptr;

                            player->setCollected(false);
                            player->resetSpeed();
                            effectTimer = 0.0f;
                        }
                    }
                    break;
                    case Item::Type::FIRE_RATE:
                    {
                        effectTimer += deltaTime;

                        if (effectTimer <= effectDuration)
                        {
                            std::cout << effectTimer << std::endl;
                            bulletCooldown = 0.05f;
                        }
                        else
                        {
                            ItemInUse->Deactivate();
                            items.returnItem(ItemInUse);
                            activeItems.erase(std::remove(activeItems.begin(), activeItems.end(), ItemInUse), activeItems.end());
                            ItemInUse = nullptr;

                            player->setCollected(false);
                            bulletCooldown = 0.2f;
                            effectTimer = 0.0f;
                        }
                    }
                    break;
                    case Item::Type::ALL_DIRECTIONAL_SHOOTING:
                    {
                        effectTimer += deltaTime;

                        if (effectTimer <= effectDuration)
                        {
                            std::cout << effectTimer << std::endl;
                            allDirectionalShooting = true;
                        }
                        else
                        {
                            ItemInUse->Deactivate();
                            items.returnItem(ItemInUse);
                            activeItems.erase(std::remove(activeItems.begin(), activeItems.end(), ItemInUse), activeItems.end());
                            ItemInUse = nullptr;

                            player->setCollected(false);
                            effectTimer = 0.0f;
                            allDirectionalShooting = false;
                        }
                    }
                    break;
                    default:
                        break;

                    }
                }
            }
        }
    }

    void Game::UpdateScoreDisplay(Surface* screen)  
    {  
       std::string scoreString = "SCORE: " + std::to_string(player->getScore());
       char* scoreCStr = const_cast<char*>(scoreString.c_str()); //converts C++ strings into C-style strings, creating a character array (char*) with the same data
       screen->PrintScaled(scoreCStr, 340, 30, 3, 3, 0xFFFFFFF); 

    }

    void Game::ShowFinalScore(Surface* screen)
    {
       std::string scoreString = "FINAL SCORE: " + std::to_string(player->getScore());
       char* scoreCStr = const_cast<char*>(scoreString.c_str()); //converts C++ strings into C-style strings, creating a character array (char*) with the same data
       screen->Centre(scoreCStr, 200, 3, 3, 0xFF0000);

    }
}