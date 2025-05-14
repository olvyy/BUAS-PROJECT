#include "bullet.hpp"

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

	if (position.x < 80 + size.x || position.x > 720 - size.x || position.y < 80 + size.y || position.y > 720 - size.y)
	{
		Deactivate();
	}
}

void bullet::Draw(Tmpl8::Surface* screen)
{
	if (bulletSprite && active)
	{
		bulletSprite->DrawScaled(static_cast<int>(position.x), static_cast<int>(position.y), size.x, size.y, screen);
	}
}

Rectangle bullet::getHitbox()
{
	hitbox.origin = position;
	hitbox.size = size;
	return hitbox;
}