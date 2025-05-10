#pragma once
#include "entity.hpp"
#include "template.h"

class Enemy : public Entity
{
public:
	Enemy();  // Default constructor
	Enemy(const Tmpl8::vec2& c_origin, std::shared_ptr<Tmpl8::Sprite> idleEnemy);

	//object pool
	void makeActive();
	void Die();
	void isDying() { dying = true; }
	void deactivate() { active = false; }
	bool isActive() const;
	void SetPosition(const Tmpl8::vec2& newPos);

protected:
	bool active = false;
	bool isAvoiding = false;
	
};