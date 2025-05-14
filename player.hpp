#pragma once
#include "entity.hpp"
#include "bullet.hpp"

class Player : public Entity
{
public:
	Player( const Tmpl8::vec2& c_origin,
			std::shared_ptr<Tmpl8::Sprite> c_idle,
			std::shared_ptr<Tmpl8::Sprite> c_moveDown,
			std::shared_ptr<Tmpl8::Sprite> c_moveUp,
			std::shared_ptr<Tmpl8::Sprite> c_moveLeft,
			std::shared_ptr<Tmpl8::Sprite> c_moveRight
		  );

	int getHearts() { return currentHearts; }
	int increaseScore();
	int decreaseScore();
	int getScore() { return currentScore; }
	void setScore(int score) { currentScore = score; }
	void takeDamage(int damage);
	void increaseHearts(int amount) { currentHearts += amount; }
	void drawHearts(Tmpl8::Surface* screen, std::shared_ptr<Tmpl8::Sprite> heartSprite, std::shared_ptr<Tmpl8::Sprite> lastTry, float deltaTime);
	void resetHearts() { currentHearts = maxHearts = 3; }
	void setCollected(bool value) { collected = value; }
	bool boolCollected() { return collected; }
	void increaseSpeed() { maxSpeed = 150; }
	void resetSpeed() { maxSpeed = 100; }
	void Reset();

protected:
	bool collected = false;
	int maxHearts = 3;
	int currentHearts = 3;
	const float scaleFactor = 3.5;
	int HeartsGap = 20;
	int currentScore = 0;
};