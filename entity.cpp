#include "entity.hpp"
#include <iostream>

//constructor
Entity::Entity( const Tmpl8::vec2& c_origin, 
				const Tmpl8::vec2& c_size, 
				std::shared_ptr<Tmpl8::Sprite> c_idle, 
				std::shared_ptr<Tmpl8::Sprite> c_moveDown, 
				std::shared_ptr<Tmpl8::Sprite> c_moveUp, 
				std::shared_ptr<Tmpl8::Sprite> c_moveLeft, 
				std::shared_ptr<Tmpl8::Sprite> c_moveRight)
	: position(c_origin),hitbox(c_origin,c_size), idle(c_idle), moveDown (c_moveDown), moveUp (c_moveUp), moveLeft (c_moveLeft), moveRight (c_moveRight), currentSprite(c_idle) {}


Rectangle Entity::GetHitbox()
{
	if (currentSprite)
	{
		hitbox.origin = position;
		hitbox.size =
		{
			static_cast<float>(currentSprite->GetWidth() * scaleFactor ),
			static_cast<float>(currentSprite->GetHeight() * scaleFactor )
		};
	}
	
	return hitbox;
}

