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

	void Activate(const Tmpl8::vec2& startPos, const Tmpl8::vec2& direction);
	void Update(float deltaTime);
	void makeActive() { active = true; }
	void Draw(Tmpl8::Surface* screen);
	Rectangle getHitbox();

	bool isActive() { return active; }
	void Deactivate() { active = false; }

protected:

	Tmpl8::vec2 position;
	Tmpl8::vec2 velocity;
	Tmpl8::vec2 shootDirection;
	const float speed = 300.0f;
	bool active;
	Tmpl8::vec2 size = { 10,10 };
	Rectangle hitbox;
	std::shared_ptr<Tmpl8::Sprite> bulletSprite;
};