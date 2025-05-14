#include "Item.hpp"

Item::Item(std::shared_ptr<Tmpl8::Sprite> c_itemSprite, Tmpl8::vec2 c_position)
	: position(c_position), itemSprite(c_itemSprite), active(false) {}

Item::Item()
	: position(0, 0), itemSprite(nullptr), active(false) {}

void Item::Draw(Tmpl8::Surface* screen, std::shared_ptr<Tmpl8::Sprite> itemSprite)
{
	if(active)
	{
		itemSprite->DrawScaled(position.x, position.y, itemSprite->GetWidth() * 2.5f, itemSprite->GetHeight() * 2.5f, screen);
	}
	return;
}

Rectangle Item::getHitbox()
{
	hitbox.origin = position;
	hitbox.size = static_cast<float>(itemSprite->GetWidth()) * scaleFactor;

	return hitbox;
}

void Item::Update(float deltaTime)
{
}
