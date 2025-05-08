#pragma once
#include "template.h"
#include "Rectangle.hpp"
#include <memory>

class Item
{
public:
	Item(std::shared_ptr<Tmpl8::Sprite> c_itemSprite, Tmpl8::vec2 c_position);
	Item();

	void Draw(Tmpl8::Surface* screen, std::shared_ptr<Tmpl8::Sprite> itemSprite);
	void setPosition(Tmpl8::vec2 newPosition) { position = newPosition; }
	void setSprite(std::shared_ptr<Tmpl8::Sprite> sprite) { itemSprite = sprite; }

	Rectangle getHitbox() 
	{ 
		hitbox.origin = position;
		hitbox.size = static_cast<float>(itemSprite->GetWidth()) * scaleFactor;
		
		return hitbox;
	}

	enum class Type
	{
		SPEED_UP,
		FIRE_RATE,
		ALL_DIRECTIONAL_SHOOTING,
		HEART
	};

	std::shared_ptr<Tmpl8::Sprite> getSprite() { return itemSprite; }
	void setItemType(int type) { itemType = static_cast<Type>(type); }
	void Update(float deltaTime);
	void Activate() { active = true; }
	void Deactivate() { active = false; }
	bool isActive() { return active; }
	Type getType() { return itemType; }

private:
	Tmpl8::vec2 position;
	Rectangle hitbox;
	std::shared_ptr<Tmpl8::Sprite> itemSprite;
	bool active = false;
	float scaleFactor = 2.5f;
	Type itemType;
};