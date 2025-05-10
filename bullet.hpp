#pragma once
#include "template.h"
#include <memory>
#include <vector>
#include "surface.h"
#include "Rectangle.hpp"

class bullet 
{
public:
	bullet(std::shared_ptr<Tmpl8::Sprite> c_bulletSprite)
		: bulletSprite(c_bulletSprite), active(false) {}

	void bullet::Activate(const Tmpl8::vec2& startPos, const Tmpl8::vec2& direction)
	{
		position = startPos;
		velocity = direction * speed;
		active = true;
	}

	void bullet::Update(float deltaTime)
	{
		if (!active) return;

		position += velocity * deltaTime;

		if (position.x < 80 + size.x || position.x > 720 - size.x|| position.y < 80 + size.y || position.y > 720 - size.y)
		{
			Deactivate();
		}
	}

	void makeAtive() { active = true; }

	void Draw(Tmpl8::Surface* screen)
	{
		if (bulletSprite && active)
		{
			bulletSprite->DrawScaled(static_cast<int>(position.x), static_cast<int>(position.y), size.x, size.y, screen);
		}
	}

	bool isActive() { return active; }
	void Deactivate() { active = false; }
	Rectangle getHitbox() 
	{ 
		hitbox.origin = position;
		hitbox.size = size;
		return hitbox;
	}

protected:

	Tmpl8::vec2 position;
	Tmpl8::vec2 velocity;
	Tmpl8::vec2 shootDirection;
	float speed = 300.0f;
	bool active;
	Tmpl8::vec2 size = { 10,10 };
	Rectangle hitbox;
	std::shared_ptr<Tmpl8::Sprite> bulletSprite;
};