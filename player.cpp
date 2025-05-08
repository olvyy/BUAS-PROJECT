#include "player.hpp"
#include "SDL.h"
#include <iostream>

Player::Player(
	const Tmpl8::vec2& c_origin,
	std::shared_ptr<Tmpl8::Sprite> c_idle,
	std::shared_ptr<Tmpl8::Sprite> c_moveDown,
	std::shared_ptr<Tmpl8::Sprite> c_moveUp,
	std::shared_ptr<Tmpl8::Sprite> c_moveLeft,
	std::shared_ptr<Tmpl8::Sprite> c_moveRight
)
	: Entity(c_origin, c_idle, c_moveDown, c_moveUp, c_moveLeft, c_moveRight) {}

void Player::takeDamage(int damage)
{
	currentHearts -= damage;

	if (currentHearts < 0)
	{
		currentHearts = 0;
	}
}

void Player::drawHearts(Tmpl8::Surface* screen, std::shared_ptr<Tmpl8::Sprite> heartSprite, std::shared_ptr<Tmpl8::Sprite> lastTry, float deltaTime)
{
	int heartWidth = heartSprite->GetWidth() * scaleFactor;
	int heartHeight = heartSprite->GetHeight() * scaleFactor;

	int lastTryWidth = lastTry->GetWidth() * scaleFactor * 1.5;
	int lastTryHeight = lastTry->GetHeight() * scaleFactor * 1.5;

	int totalWidth = (currentHearts * heartWidth) + ((currentHearts - 1) * HeartsGap);
	int startX = (screen->GetWidth() - totalWidth) / 2 + heartWidth / 2;
	int heartY = screen->GetHeight() - (heartSprite->GetHeight() * scaleFactor * 1.5);

	if (currentHearts <= 0)
	{
		lastTry->SetFlags(Tmpl8::Sprite::FLASH);
		lastTry->DrawScaledAnimated(screen, screen->GetWidth() / 2 - (lastTryWidth / 2), 750, lastTryWidth, lastTryHeight);
	}

	for (int i = 0; i < currentHearts; ++i)
	{
		int x = startX + i * (heartWidth + HeartsGap);
		heartSprite->DrawScaled(x, heartY, heartWidth, heartHeight, screen);
	}
}

void Player::Reset()
{
	movingUp = movingDown = movingLeft = movingRight = false;
	currentSprite = idle;
	currentFrame = 0;
	animationTimer = 0;
	velocity = { 0,0 };
}










