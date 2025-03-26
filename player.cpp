#include "player.hpp"
#include "SDL.h"
#include <iostream>

Player::Player(
	const Tmpl8::vec2& c_origin,
	const Tmpl8::vec2& c_size,
	std::shared_ptr<Tmpl8::Sprite> c_idle,
	std::shared_ptr<Tmpl8::Sprite> c_moveDown,
	std::shared_ptr<Tmpl8::Sprite> c_moveUp,
	std::shared_ptr<Tmpl8::Sprite> c_moveLeft,
	std::shared_ptr<Tmpl8::Sprite> c_moveRight
)
	: Entity(c_origin, c_size, c_idle, c_moveDown, c_moveUp, c_moveLeft, c_moveRight) {}

//PLAYER MOVEMENT & COLLISIONS _______________________________________________________________________________________________________________
void Player::HandleInput()
{
	// get the current keyboard state
	const Uint8* keystates = SDL_GetKeyboardState(nullptr);

	movingUp = keystates[SDL_SCANCODE_W];

	movingDown = keystates[SDL_SCANCODE_S];

	movingLeft = keystates[SDL_SCANCODE_A];

	movingRight = keystates[SDL_SCANCODE_D];

	if (keystates[SDL_SCANCODE_H])
	{
		if (!hKeyPressed) // Only toggle if the key was not previously pressed
		{
			drawHitboxes = !drawHitboxes;
			hKeyPressed = true;
		}
	}
	else
	{
		hKeyPressed = false;
	}
}

void Player::Update(float deltaTime, const Rectangle& playerHitbox, const std::vector<Rectangle>& tiles)
{
	float speed = 2.5f;
	velocity = { 0,0 };

	if (movingUp)
	{
		velocity.y = -speed;
		currentSprite = moveUp;
	}
	else if (movingDown)
	{
		velocity.y = +speed;
		currentSprite = moveDown;
	}
	if (movingLeft)
	{
		velocity.x = -speed;
		currentSprite = moveLeft;
	}
	else if (movingRight)
	{
		velocity.x = +speed;
		currentSprite = moveRight;
	}

	Tmpl8::vec2 newPosition = position + velocity;

	if (newPosition.x > (720 - (currentSprite->GetWidth() * scaleFactor)) || newPosition.x < 80 ||
		newPosition.y > (720 - (currentSprite->GetWidth() * scaleFactor)) || newPosition.y < 80 )
	{
		velocity = 0;
		std::cout << "screen bound" << std::endl;
		return;
	}

	Rectangle newHitbox(newPosition, hitbox.size);

    // Check for collisions with tiles
    for (const Rectangle& tile : tiles)
    {
        if (collide(newHitbox, tile))
        {
			std::cout << "collision" << std::endl;
			velocity = 0;
			return;
        }
    }

	position.x += velocity.x;
	position.y += velocity.y;

    if (velocity.x == 0 && velocity.y == 0)
    {
        currentSprite = idle;
        currentFrame = 0;
        currentSprite->SetFrame(currentFrame);
    }
}

void Player::drawPlayer(Tmpl8::Surface* screen, float deltaTime)
{

	updateAnimation(deltaTime); 

	if (currentSprite)
	{
		currentSprite->DrawScaledAnimated(screen, static_cast<int>(position.x), static_cast<int>(position.y), newWidth, newHeight);
	}

	if (drawHitboxes == true)
	{
		DrawRectangle(GetHitbox(), screen, 0x0000FF);
	}
}

void Player::updateAnimation(float deltaTime)
{
	animationTimer += deltaTime;

	if (animationTimer >= animationSpeed)
	{
		animationTimer = 0;
		currentFrame = (currentFrame + 1) % currentSprite->Frames();
		currentSprite->SetFrame(currentFrame);
	}
}







