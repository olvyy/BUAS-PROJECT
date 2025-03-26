#pragma once
#include "surface.h"
#include <memory> //to be able to use a shared pointer
#include "template.h"
#include "Rectangle.hpp"

class Entity
{
public:
	//constructor
	Entity( const Tmpl8::vec2& c_origin,
			const Tmpl8::vec2& c_size,
			std::shared_ptr<Tmpl8::Sprite> c_idle,
			std::shared_ptr<Tmpl8::Sprite> c_moveDown,
			std::shared_ptr<Tmpl8::Sprite> c_moveUp,
			std::shared_ptr<Tmpl8::Sprite> c_moveLeft,
			std::shared_ptr<Tmpl8::Sprite> c_moveRight
			);																//c_ = constructor parameter

	virtual ~Entity() = default;

	Rectangle GetHitbox();

protected:
	std::shared_ptr<Tmpl8::Sprite> idle = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> moveDown = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> moveUp = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> moveLeft = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> moveRight = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> currentSprite = { nullptr };

	float scaleFactor = 2.5f;
	Tmpl8::vec2 position; //position of entity
	Tmpl8::vec2 size; //size of entity for hitbox
	Rectangle hitbox;
};