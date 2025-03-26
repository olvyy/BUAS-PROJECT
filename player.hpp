#pragma once
#include "entity.hpp"
#include <iostream>
#include <vector>

class Player : public Entity
{
public:
	//constructor
	Player( const Tmpl8::vec2& c_origin,
			const Tmpl8::vec2& c_size,
			std::shared_ptr<Tmpl8::Sprite> c_idle,
			std::shared_ptr<Tmpl8::Sprite> c_moveDown,
			std::shared_ptr<Tmpl8::Sprite> c_moveUp,
			std::shared_ptr<Tmpl8::Sprite> c_moveLeft,
			std::shared_ptr<Tmpl8::Sprite> c_moveRight
		  );

	//updating
	void HandleInput();
	void Update(float deltaTime,const Rectangle& playerHitbox, const std::vector<Rectangle>& tiles);
	void updateAnimation(float deltaTime);

	//drawing
	void drawPlayer(Tmpl8::Surface* screen, float deltaTime);
	bool hKeyPressed = { false };
	bool drawHitboxes = { false };

	//public variables
	Tmpl8::vec2 velocity = { 0,0 };
	bool movingUp = false;
	bool movingDown = false;
	bool movingLeft = false;
	bool movingRight = false;

	//animation
	float animationTimer = 0.0f;
	float animationSpeed = 150.0f;
	int currentFrame = 0;

protected:
	int newWidth = static_cast<int>(currentSprite->GetWidth() * scaleFactor);
	int newHeight = static_cast<int>(currentSprite->GetHeight() * scaleFactor);
};