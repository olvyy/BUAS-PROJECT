#include "entity.hpp"
#include "Line.hpp"
#include <iostream>
#include "SDL.h"
#include <cmath>

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

		Tmpl8::vec2 steering = { 0, 0 };
		Tmpl8::vec2 seek = Tmpl8::vec2::normalize(player->position - position) * maxSpeed;
		Tmpl8::vec2 avoidance = CalculateAvoidance(tiles6, screen);

		float avoidanceWeight = 0.9f;
		float seekingWeight = 1.0f - avoidanceWeight;

		steering = avoidance * avoidanceWeight + seek * seekingWeight;

		steering = steering.Truncate(MAX_FORCE);

		velocity = velocity + steering;
		velocity = velocity.Truncate(MAX_FORCE);
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
		Rectangle newHitbox(newPosition, hitbox.size - 5); //-5 to make the collision check less harsh and avoid getting stuck on walls


		for (const auto& otherEnemy : enemies)
		{
			if (this != otherEnemy.get())
			{
				if (collide(newHitbox, otherEnemy->GetHitbox()))
				{
					Tmpl8::vec2 repulsion = (position - otherEnemy->getPosition()).normalized() * AVOID_FORCE;
					velocity += repulsion;
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
				if (aiControlled)
				{
					Tmpl8::vec2 repulsion = (position - tile.Center()).normalized() * AVOID_FORCE;
					velocity += repulsion;
				}
				else
				{
					velocity = {0,0};
				}
				return;
			}
		}
	}

	position += velocity * deltaTime;
}

/////////////////////////////////////////////////
/////				DRAWING				////////
///////////////////////////////////////////////

Tmpl8::vec2 Entity::CalculateAvoidance(std::vector<Rectangle>& obstacles, Tmpl8::Surface* screen)
{
	Tmpl8::vec2 positionCenter = position + hitbox.size * 0.5f;

	Tmpl8::vec2 ahead = positionCenter + Tmpl8::vec2::normalize(velocity) * SEE_AHEAD;
	/*screen->Line(positionCenter.x, positionCenter.y, ahead.x, ahead.y, 0xFF0000);*/

	Rectangle* mostThreatening = nullptr;
	Tmpl8::vec2 avoidance = { 0, 0 };

	// Find the most threatening obstacle
	for (auto& obstacle : obstacles)
	{
		if (LineIntersectsRectangle(positionCenter, ahead, obstacle, screen))
		{
			if (!mostThreatening || (obstacle.Center() - positionCenter).length() < (mostThreatening->Center() - positionCenter).length())
			{
				mostThreatening = &obstacle;
				/*screen->Box(obstacle.origin.x, obstacle.origin.y,
					obstacle.origin.x + obstacle.size.x,
					obstacle.origin.y + obstacle.size.y, 0xFF0000);*/
			}
		}
	}

	if (mostThreatening)
	{
		avoidance = Tmpl8::vec2::normalize(ahead - mostThreatening->Center()) * AVOID_FORCE;

		/*screen->Line(mostThreatening->Center().x, mostThreatening->Center().y,
			mostThreatening->Center().x + avoidance.x,
			mostThreatening->Center().y + avoidance.y, 0x00FF00);*/
	}

	return avoidance;
}

bool Entity::LineIntersectsRectangle(const Tmpl8::vec2& start, const Tmpl8::vec2& end, const Rectangle& rect, Tmpl8::Surface* screen)
{
	Tmpl8::vec2 topLeft = rect.origin;
	Tmpl8::vec2 topRight = { rect.origin.x + rect.size.x, rect.origin.y };
	Tmpl8::vec2 bottomLeft = { rect.origin.x, rect.origin.y + rect.size.y };
	Tmpl8::vec2 bottomRight = rect.origin + rect.size;


	return	LineLineIntersection(start, end, topLeft, topRight, screen) ||
			LineLineIntersection(start, end, topRight, bottomRight, screen) ||
			LineLineIntersection(start, end, bottomRight, bottomLeft, screen) ||
			LineLineIntersection(start, end, bottomLeft, topLeft, screen);
}

bool Entity::LineLineIntersection(Tmpl8::vec2 a1, Tmpl8::vec2 a2, Tmpl8::vec2 b1, Tmpl8::vec2 b2, Tmpl8::Surface* screen)
{
	Tmpl8::vec2 s1 = a2 - a1; // Direction of line 1
	Tmpl8::vec2 s2 = b2 - b1; // Direction of line 2

	float denominator = (-s2.x * s1.y + s1.x * s2.y);
	if (denominator == 0)
	{
		return false; // Lines are parallel
	}

	float s = (-s1.y * (a1.x - b1.x) + s1.x * (a1.y - b1.y)) / denominator;
	float t = (s2.x * (a1.y - b1.y) - s2.y * (a1.x - b1.x)) / denominator;


	return (s >= 0 && s <= 1 && t >= 0 && t <= 1);
}

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

