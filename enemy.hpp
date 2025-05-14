#pragma once
#include "entity.hpp"
#include "template.h"

class Enemy : public Entity
{
public:
	Enemy();
	Enemy(const Tmpl8::vec2& c_origin, std::shared_ptr<Tmpl8::Sprite> idleEnemy);

	void makeActive();
	void Die();
	bool isActive() const;
	void SetPosition(const Tmpl8::vec2& newPos);

	void isDying() { dying = true; }
	void deactivate() { active = false; dying = false; }
	
protected:
	bool active = false;
	bool isAvoiding = false;
	
};