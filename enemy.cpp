#include "enemy.hpp"

Enemy::Enemy()
	: Entity(Tmpl8::vec2(0, 0), nullptr, nullptr, nullptr, nullptr, nullptr), active(false) {}

Enemy::Enemy(const Tmpl8::vec2& c_origin, std::shared_ptr<Tmpl8::Sprite> idleEnemy)
	: Entity(c_origin, idleEnemy, idleEnemy, idleEnemy, idleEnemy, idleEnemy), active(false)
{
	aiControlled = true;
}

void Enemy::makeActive()
{
	active = true;
}

void Enemy::Die()
{
	if (dying || !active) return;

	dying = true;
	active = false;
}

bool Enemy::isActive() const
{
	return active;
}

void Enemy::SetPosition(const Tmpl8::vec2& newPos)
{
	position = newPos;
}