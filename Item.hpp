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
	void Update(float deltaTime);

	Rectangle getHitbox();
	std::shared_ptr<Tmpl8::Sprite> getSprite() { return itemSprite; }


	void setPosition(Tmpl8::vec2 newPosition) { position = newPosition; }
	void setSprite(std::shared_ptr<Tmpl8::Sprite> sprite) { itemSprite = sprite; }
	void setItemType(int type) { itemType = static_cast<Type>(type); }

	enum class Type
	{
		SPEED_UP,
		FIRE_RATE,
		ALL_DIRECTIONAL_SHOOTING,
		HEART
	};
	Type getType() { return itemType; }

	void Activate() { active = true; }
	void Deactivate() { active = false; }
	bool isActive() { return active; }

private:
	Tmpl8::vec2 position;
	Rectangle hitbox;
	std::shared_ptr<Tmpl8::Sprite> itemSprite;
	bool active = false;
	float scaleFactor = 2.5f;
	Type itemType;
};