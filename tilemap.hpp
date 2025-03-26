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

	//drawing
	void DrawMap(Tmpl8::Surface* surface, float scaleFactor, float deltaTime);
	void UpdateTileAnimation(float deltaTime);
	bool hKeyPressed = { false };
	bool drawHitboxes = { false };
	
	//hitbox
	const std::vector<Rectangle>& GetTileHitboxes() const { return tileHitboxes; }

protected:
	Rectangle src, dest;
	int map[16][16];
	float scaleFactor;

	//tiles
	std::shared_ptr<Tmpl8::Sprite> tile0, tile1, tile2, tile3, tile4, tile5, tile6 = { nullptr };

	//tile animation
	float tileAnimationTimer = 0.0f;
	int tileCurrentFrame = 0;
	float tileAnimationSpeed = 750.0f;

	//hitboxes
	std::vector<Rectangle> tileHitboxes;
};
