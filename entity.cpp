#include "entity.hpp"
#include "Line.hpp"
#include <iostream>
#include "SDL.h"
#include <cmath>

//seek and avoid behaviour inspired by: https://code.tutsplus.com/understanding-steering-behaviors-collision-avoidance--gamedev-7777t
// and https://code.tutsplus.com/understanding-steering-behaviors-seek--gamedev-849t

Entity::Entity( const Tmpl8::vec2& c_origin, 
				std::shared_ptr<Tmpl8::Sprite> c_idle, 
				std::shared_ptr<Tmpl8::Sprite> c_moveDown, 
				std::shared_ptr<Tmpl8::Sprite> c_moveUp, 
				std::shared_ptr<Tmpl8::Sprite> c_moveLeft, 
				std::shared_ptr<Tmpl8::Sprite> c_moveRight)
	: position(c_origin), idle(c_idle), moveDown (c_moveDown), moveUp (c_moveUp), moveLeft (c_moveLeft), moveRight (c_moveRight), currentSprite(c_idle), aiControlled(false) {}

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

/////////////////////////////////////////////////
/////			MOVMENT & COLLISION		////////
///////////////////////////////////////////////

void Entity::Update(float deltaTime,
	std::vector<Rectangle>& tiles,
	std::vector<Rectangle>& tiles6,
	std::vector<std::shared_ptr<Entity>>& enemies,
	std::shared_ptr<Entity>& player,
	Tmpl8::Surface* screen)
{
	//enemies
	if (aiControlled)
	{
		if (dying)
		{
			DeathAnimationTimer += deltaTime;

			if (DeathAnimationTimer >= DeathAnimationSpeed)
			{
				deathCurrentFrame++;
				DeathAnimationTimer = 0.0f;
			}

			if (deathCurrentFrame >= 6)
			{
				dying = false;
				deathCurrentFrame = 0;
				DeathAnimationTimer = 0.0f;

				returnToPool = true;
			}

			return;
		}

		bool isAvoiding = false;
		Tmpl8::vec2 avoidForce = CalculateAvoidance(tiles6, screen, isAvoiding);

		// Always calculate seek force
		Tmpl8::vec2 seekForce = (player->position - position).normalized() * maxSpeed;

		// Blend forces dynamically
		Tmpl8::vec2 steering;
		if (isAvoiding)
		{
			// Prioritize avoidance but still curve toward the player
			steering = (avoidForce * 0.8f) + (seekForce * 0.3f);
		}
		else
		{
			// No avoidance needed, fully seek the player
			steering = seekForce;
		}

		// Update velocity and clamp to max speed
		velocity = velocity.Truncate(velocity + steering, maxSpeed);

		//screen->Line(position.x, position.y, velocity.x + position.x, velocity.y + position.y, 0xff0000);
	}

	//player
	{
		if (isInvincible)
		{
			damageCooldown += deltaTime;

			if (damageCooldown >= damageCooldownTime)
			{
				isInvincible = false;
				damageCooldown = 0.0f;
				currentSprite->SetFlags(Tmpl8::Sprite::FLASH);
			}
			else
			{
				currentSprite->SetFlags(Tmpl8::Sprite::FLASH);
			}
		}

		if (!aiControlled)
		{
			velocity = { 0,0 };

			if (movingUp)
			{
				velocity.y = -maxSpeed;
				currentSprite = moveUp;
				//std::cout << "player velocity: " << velocity.y << std::endl;
			}
			else if (movingDown)
			{
				velocity.y = +maxSpeed;
				currentSprite = moveDown;
				//std::cout << "player velocity: "<< velocity.y << std::endl;
			}
			if (movingLeft)
			{
				velocity.x = -maxSpeed;
				currentSprite = moveLeft;
				//std::cout << "player velocity: " << velocity.x << std::endl;
			}
			else if (movingRight)
			{
				velocity.x = +maxSpeed;
				currentSprite = moveRight;
				//std::cout << "player velocity: " << velocity.x << std::endl;
			}
			if (velocity.x == 0 && velocity.y == 0)
			{
				currentSprite = idle;
				currentFrame = 0;
				currentSprite->SetFrame(currentFrame);
			}

			if (movingDown && movingLeft || movingDown && movingRight || movingUp && movingLeft || movingUp && movingRight)
			{
				velocity *= 0.7071f; //normalize diagonal movement
			}
		}
	}
	
	//collisions
	{
		Tmpl8::vec2 newPosition = position + velocity * deltaTime;
		Rectangle newHitbox(newPosition, hitbox.size);


		for (const auto& otherEnemy : enemies)
		{
			if (this != otherEnemy.get())
			{
				if (collide(newHitbox, otherEnemy->GetHitbox()))
				{
					isColliding = true;
					velocity = 0;
					return;
				}
			}
		}

		if (newPosition.x > (720 - (currentSprite->GetWidth() * scaleFactor)) || newPosition.x < 80 ||
			newPosition.y >(720 - (currentSprite->GetWidth() * scaleFactor)) || newPosition.y < 80)
		{
			velocity = 0;
			return;
		}

		for (const Rectangle& tile : tiles)
		{
			if (collide(newHitbox, tile))
			{
				velocity = 0;
				return;
			}
		}
	}

	position += velocity * deltaTime;
}

/////////////////////////////////////////////////
/////			COLLISION AVOIDANCE		////////
///////////////////////////////////////////////

Tmpl8::vec2 Entity::CalculateAvoidance(std::vector<Rectangle>& obstacles, Tmpl8::Surface* screen, bool& isAvoiding)
{
	Tmpl8::vec2 enemyCenter = { position.x + hitbox.size.x / 2,
								position.y + hitbox.size.y / 2 };

	Tmpl8::vec2 direction = velocity.length() > 0 ? velocity.normalized() : Tmpl8::vec2(0, 0);

	Tmpl8::vec2 ahead = enemyCenter + direction * SEE_AHEAD;
	Tmpl8::vec2 ahead2 = enemyCenter + direction * (SEE_AHEAD * 0.5f);

	screen->Line(enemyCenter.x, enemyCenter.y, ahead.x, ahead.y, 0x000000);

	Rectangle* mostThreatening = nullptr;
	isAvoiding = false;

	for (const auto& obstacle : obstacles)
	{
		if (LineIntersectsRectangle(enemyCenter, ahead, obstacle, screen) ||
			LineIntersectsRectangle(enemyCenter, ahead2, obstacle, screen))
		{
			if (!mostThreatening || Tmpl8::vec2::distance(enemyCenter, obstacle.Center()) <
				Tmpl8::vec2::distance(enemyCenter, mostThreatening->Center()))
			{
				mostThreatening = const_cast<Rectangle*>(&obstacle);
			}
		}
	}

	if (mostThreatening)
	{
		isAvoiding = true;
		Tmpl8::vec2 center = { mostThreatening->origin.x + mostThreatening->size.x / 2,
							   mostThreatening->origin.y + mostThreatening->size.y / 2 };

		Tmpl8::vec2 avoidance = (enemyCenter - center).normalized() * AVOID_FORCE;
		screen->Line(center.x, center.y, center.x + avoidance.x, center.y + avoidance.y, 0xffffff);

		screen->Box(mostThreatening->origin.x, mostThreatening->origin.y,
			mostThreatening->size.x + mostThreatening->origin.x,
			mostThreatening->size.y + mostThreatening->origin.y, 0x00FF00);

		return avoidance;
	}

	return { 0, 0 };
}

bool Entity::LineIntersectsRectangle(const Tmpl8::vec2& start, const Tmpl8::vec2& end, const Rectangle& rect, Tmpl8::Surface* screen)
{
	Line line;
	line.base = start;
	line.direciton = end - start;
	//screen->Line(line.base.x, line.base.y, line.direciton.x + line.base.x, line.direciton.y + line.base.y, 0x0000FF);
	return line.LineRectangleCollision(line, rect);
}

/////////////////////////////////////////////////
/////				DRAWING				////////
///////////////////////////////////////////////

void Entity::Draw(Tmpl8::Surface* screen, float deltaTime)
{
	//for hitboxes
	const Uint8* keystates = SDL_GetKeyboardState(nullptr);

	if (keystates[SDL_SCANCODE_H])
	{
		if (!hKeyPressed)
		{
			drawHitboxes = !drawHitboxes;
			hKeyPressed = true;
		}
	}
	else
	{
		hKeyPressed = false;
	}

	if (dying && deathSprite)
	{
		deathSprite->SetFrame(deathCurrentFrame);
		deathSprite->DrawScaledAnimated(screen, static_cast<int>(position.x), static_cast<int>(position.y), newWidth, newHeight);
		
		return;
	}

	if (currentSprite)
	{
		currentSprite->DrawScaledAnimated(screen, static_cast<int>(position.x), static_cast<int>(position.y), newWidth, newHeight);
	}

	if (drawHitboxes == true)
	{
		DrawRectangle(GetHitbox(), screen, 0x0000FF);
	}

	updateAnimation(deltaTime);

}

void Entity::updateAnimation(float deltaTime)
{
	animationTimer += deltaTime;
	if (animationTimer >= animationSpeed)
	{
		currentSprite->SetFrame(currentFrame);
		currentFrame++;
		animationTimer = 0.0f;
	}
	if (currentFrame >= currentSprite->Frames())
	{
		currentFrame = 0;
	}

	newWidth = static_cast<int>(currentSprite->GetWidth() * scaleFactor);
	newHeight = static_cast<int>(currentSprite->GetHeight() * scaleFactor);

}
