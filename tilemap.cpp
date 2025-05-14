//followed a tutorial for this: https://www.youtube.com/watch?v=1eaxE_waDNc&t=1124s&ab_channel=Let%27sMakeGames

#include "tilemap.hpp"
#include "surface.h"
#include "SDL.h"

#include <random>
#include <iostream>

GameMap::GameMap()
{
	LoadMap(lvl1);

	tile0 = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/tile0.png"), 1);
	tile1 = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/tile1.png"), 1);
	tile2 = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/tile2.png"), 1);
	tile3 = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/tile3.png"), 1);
	tile4 = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/tile4.png"), 1);
	tile5 = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/tile5.png"), 2);
	tile6 = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/tile6.png"), 1);

	src.origin = dest.origin = { 0,0 };
	src.size = dest.size = { 16,16 };
}

void GameMap::LoadMap(int arr[16][16])
{
	for (int row = 0; row < 16; row++)
	{
		for (int column = 0; column < 16; column++)
		{
			map[row][column] = arr[row][column];
		}
	}
}

void GameMap::DrawMap(Tmpl8::Surface* screen, float scaleFactor, float deltaTime)
{
	float mapWdith = 16 * tileSize * scaleFactor;
	float mapHeight = 16 * tileSize * scaleFactor;

	float Xoffset = ((screen->GetWidth() - mapWdith) / 2);
	float Yoffset = ((screen->GetHeight() - mapHeight) / 2);

	tileHitboxes.clear();
	tileHitboxes6.clear();

	for (int row = 0; row < 16; row++)
	{
		for (int column = 0;column < 16; column++)
		{
			int tileID = map[row][column];

			dest.origin = { Xoffset + static_cast<float>(column * tileSize * scaleFactor), Yoffset + static_cast<float>(row * tileSize * scaleFactor) };
			dest.size = { static_cast<float>(tileSize * scaleFactor), static_cast<float>(tileSize * scaleFactor) };

			switch (tileID)
			{
			case 0:
			{
				tile0->DrawScaled(static_cast<int>(dest.origin.x), static_cast<int>(dest.origin.y), static_cast<int>(dest.size.x), static_cast<int>(dest.size.y), screen);
				break;
			}
			case 1:
				tile1->DrawScaled(static_cast<int>(dest.origin.x), static_cast<int>(dest.origin.y), static_cast<int>(dest.size.x), static_cast<int>(dest.size.y), screen);
				break;

			case 2:
				tile2->DrawScaled(static_cast<int>(dest.origin.x), static_cast<int>(dest.origin.y), static_cast<int>(dest.size.x), static_cast<int>(dest.size.y), screen);
				break;
			case 3:
				tile3->DrawScaled(static_cast<int>(dest.origin.x), static_cast<int>(dest.origin.y), static_cast<int>(dest.size.x), static_cast<int>(dest.size.y), screen);
				break;
			case 4:
				tile4->DrawScaled(static_cast<int>(dest.origin.x), static_cast<int>(dest.origin.y), static_cast<int>(dest.size.x), static_cast<int>(dest.size.y), screen);
				break;
			case 5:
			{
				tile5->DrawScaledAnimated(screen, static_cast<int>(dest.origin.x), static_cast<int>(dest.origin.y), static_cast<int>(dest.size.x), static_cast<int>(dest.size.y));
				tileHitboxes.emplace_back(dest.origin, dest.size);
				break;
			}
			case 6:
			{
				tile6->DrawScaled(static_cast<int>(dest.origin.x), static_cast<int>(dest.origin.y), static_cast<int>(dest.size.x), static_cast<int>(dest.size.y), screen);
				tileHitboxes.emplace_back(dest.origin, dest.size);
				tileHitboxes6.emplace_back(dest.origin, dest.size);
				break;
			}
			}
		}
	}
}

void GameMap::UpdateTileAnimation(float deltaTime)
{
	tileAnimationTimer += deltaTime;

	if (tileAnimationTimer >= tileAnimationSpeed)
	{
		tileCurrentFrame = (tileCurrentFrame + 1) % tile5->Frames(); 
		tile5->SetFrame(tileCurrentFrame); 
		tileAnimationTimer = 0.0f;
	}
}
