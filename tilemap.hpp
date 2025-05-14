#pragma once
#include "game.h"
#include "Rectangle.hpp"
#include <vector>

class GameMap
{
public:
	GameMap();
	~GameMap() {};

	void LoadMap(int arr[16][16]);
	void DrawMap(Tmpl8::Surface* surface, float scaleFactor, float deltaTime);
	void UpdateTileAnimation(float deltaTime);
	
	const std::vector<Rectangle>& GetTileHitboxes() const { return tileHitboxes; }
	const std::vector<Rectangle>& Gettiles6() const { return tileHitboxes6; }

	int lvl1[16][16] = {
		{5,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5},
		{5,5,5,2,2,2,2,2,2,2,2,2,2,5,5,5},
		{5,2,3,3,3,3,3,3,3,3,3,3,3,3,5,5},
		{5,2,3,3,3,3,3,3,3,3,3,3,3,3,2,5},
		{5,2,3,3,6,3,3,4,3,3,3,6,3,3,2,5},
		{5,2,3,3,3,4,3,3,4,3,3,3,3,3,2,5},
		{5,2,3,3,3,3,4,3,3,3,3,3,3,3,2,0},
		{0,2,3,3,3,3,3,3,3,3,3,3,3,3,2,0},
		{0,2,3,3,3,3,3,3,3,3,3,4,3,3,2,0},
		{0,2,3,3,4,3,3,3,3,3,3,3,3,3,2,5},
		{5,2,3,3,6,3,3,3,3,3,3,3,3,3,2,5},
		{5,2,3,3,6,6,3,3,3,3,3,6,3,3,2,5},
		{5,2,3,3,3,3,3,3,3,3,3,3,3,4,2,5},
		{5,5,3,3,4,3,3,3,3,3,3,3,3,3,5,5},
		{5,5,5,2,2,2,2,2,2,2,2,2,2,5,5,5},
		{5,5,5,5,5,5,5,0,0,0,5,5,5,5,5,5}
	};

protected:
	Rectangle src, dest;
	int map[16][16];
	float scaleFactor;
	const int tileSize = 16;

	std::shared_ptr<Tmpl8::Sprite> tile0, tile1, tile2, tile3, tile4, tile5, tile6 = { nullptr };

	float tileAnimationTimer = 0.0f;
	int tileCurrentFrame = 0;
	float tileAnimationSpeed = 0.50f;

	std::vector<Rectangle> tileHitboxes;
	std::vector<Rectangle> tileHitboxes6;
};
