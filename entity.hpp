#pragma once
#include "surface.h"
#include "template.h"
#include "Rectangle.hpp"

#include <memory> //to be able to use a shared pointer
#include <vector>

class Entity
{
public:
	Entity( const Tmpl8::vec2& c_origin,
			std::shared_ptr<Tmpl8::Sprite> c_idle,
			std::shared_ptr<Tmpl8::Sprite> c_moveDown,
			std::shared_ptr<Tmpl8::Sprite> c_moveUp,
			std::shared_ptr<Tmpl8::Sprite> c_moveLeft,
			std::shared_ptr<Tmpl8::Sprite> c_moveRight
			);																//c_ = constructor parameter

	virtual ~Entity() = default;

	//drawing
	void Entity::Update(float deltaTime,
		std::vector<Rectangle>& tiles,
		std::vector<Rectangle>& tiles6,
		std::vector<std::shared_ptr<Entity>>& entities,
		std::shared_ptr<Entity>& player,
		Tmpl8::Surface* screen);

	//collision avoidance
	float maxSpeed = 100.0f;
	const float SEE_AHEAD = 140.0f;
	const float MAX_FORCE = 80.0f;
	const float AVOID_FORCE = 100.0f;
	Tmpl8::vec2 Entity::CalculateAvoidance(std::vector<Rectangle>& obstacles, Tmpl8::Surface* screen);
	bool Entity::LineIntersectsRectangle(const Tmpl8::vec2& start, const Tmpl8::vec2& end, const Rectangle& rect, Tmpl8::Surface* screen);
	bool Entity::LineLineIntersection(Tmpl8::vec2 a1, Tmpl8::vec2 a2, Tmpl8::vec2 b1, Tmpl8::vec2 b2, Tmpl8::Surface* screen);
	

	//main functions
	void Draw(Tmpl8::Surface* screen, float deltaTime);
	void updateAnimation(float deltaTime);

	//setters
	void setPosition(Tmpl8::vec2 newPosition) { position = newPosition; }
	void setVelocity(Tmpl8::vec2 newVelocity) { velocity = newVelocity; }
	void SetIdle(std::shared_ptr<Tmpl8::Sprite> newIdle) { idle = newIdle; }
	void makeInvincible() { isInvincible = true; }

	//getters
	Tmpl8::vec2 getPosition() { return position; }	
	Tmpl8::vec2 getVelocity() { return velocity; }
	bool readytoPool() { return returnToPool; }
	bool isDying() { return dying; }
	bool getInvincible() { return isInvincible; }
	std::shared_ptr<Tmpl8::Sprite> getIdle() const { return idle; }
	bool hasPlayedDeathSound() { return playedDeathSound; }
	void setPlayedDeathSound(bool value) { playedDeathSound = value; }
	
	//hitboxes
	bool drawHitboxes = { false };
	bool hKeyPressed = { false };
	bool isColliding = { false };
	Rectangle GetHitbox();

	//animation
	float animationTimer = 0.0f;
	float animationSpeed = 0.2f;
	int currentFrame = 0;

	//player
	bool movingUp = false;
	bool movingDown = false;
	bool movingLeft = false;
	bool movingRight = false;

	std::shared_ptr<Tmpl8::Sprite> deathSprite = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/death.png"), 6);

protected:
	std::shared_ptr<Tmpl8::Sprite> idle = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> moveDown = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> moveUp = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> moveLeft = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> moveRight = { nullptr };
	std::shared_ptr<Tmpl8::Sprite> currentSprite = { nullptr };

	int newWidth = 0;
	int newHeight = 0;
	float scaleFactor = 2.5f;

	//enemy
	bool playedDeathSound = false;
	bool aiControlled;
	bool dying = false;
	bool returnToPool = false;
	float DeathAnimationTimer = 0.0f;
	float DeathAnimationSpeed = 0.1f;
	int deathCurrentFrame = 0;

	//player
	float damageCooldown = 0.0f;
	const float damageCooldownTime = 2.0f;
	bool isInvincible = false;

	Tmpl8::vec2 position; 
	Tmpl8::vec2 velocity;
	Tmpl8::vec2 size; 
	Rectangle hitbox;
};