#pragma once  
#include "surface.h"  
#include "template.h"  
#include "Rectangle.hpp"  

#include <memory>  
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
		);  

~Entity() = default;  

void Entity::Update(float deltaTime,  
	std::vector<Rectangle>& tiles,  
	std::vector<Rectangle>& tiles6,  
	std::vector<std::shared_ptr<Entity>>& entities,  
	std::shared_ptr<Entity>& player,  
	Tmpl8::Surface* screen);  
void Draw(Tmpl8::Surface* screen, float deltaTime);  
void updateAnimation(float deltaTime);  
Rectangle GetHitbox();  

//COLLISION AVOIDANCE  
Tmpl8::vec2 Entity::CalculateAvoidance(std::vector<Rectangle>& obstacles, Tmpl8::Surface* screen);  
bool Entity::LineIntersectsRectangle(const Tmpl8::vec2& start, const Tmpl8::vec2& end, const Rectangle& rect, Tmpl8::Surface* screen);  
bool Entity::LineLineIntersection(Tmpl8::vec2 a1, Tmpl8::vec2 a2, Tmpl8::vec2 b1, Tmpl8::vec2 b2, Tmpl8::Surface* screen);  

//SETTERS  
void setPosition(Tmpl8::vec2 newPosition) { position = newPosition; }  
void setVelocity(Tmpl8::vec2 newVelocity) { velocity = newVelocity; }  
void SetIdle(std::shared_ptr<Tmpl8::Sprite> newIdle) { idle = newIdle; }  
void makeInvincible() { isInvincible = true; }  
void setInvincible(bool value) { isInvincible = value; }  
void setPlayedDeathSound(bool value) { playedDeathSound = value; }  
void setdeathSprite(std::shared_ptr<Tmpl8::Sprite> sprite) { deathSprite = sprite; }  

//GETTERS  
Tmpl8::vec2 getPosition() { return position; }	  
Tmpl8::vec2 getVelocity() { return velocity; }  
bool readytoPool() { return returnToPool; }  
bool isDying() { return dying; }  
bool getInvincible() { return isInvincible; }  
std::shared_ptr<Tmpl8::Sprite> getIdle() const { return idle; }  
bool hasPlayedDeathSound() { return playedDeathSound; }  

bool movingUp = false;  
bool movingDown = false;  
bool movingLeft = false;  
bool movingRight = false;  

protected:  
std::shared_ptr<Tmpl8::Sprite> idle = { nullptr };  
std::shared_ptr<Tmpl8::Sprite> moveDown = { nullptr };  
std::shared_ptr<Tmpl8::Sprite> moveUp = { nullptr };  
std::shared_ptr<Tmpl8::Sprite> moveLeft = { nullptr };  
std::shared_ptr<Tmpl8::Sprite> moveRight = { nullptr };  
std::shared_ptr<Tmpl8::Sprite> currentSprite = { nullptr };  
std::shared_ptr<Tmpl8::Sprite> deathSprite = { nullptr };  

//SCALING  
int newWidth = 0;  
int newHeight = 0;  
const float scaleFactor = 2.5f;	  

bool playedDeathSound = false;  
bool aiControlled;  
bool dying = false;  
bool returnToPool = false;  
float DeathAnimationTimer = 0.0f;  
float DeathAnimationSpeed = 0.1f;  
int deathCurrentFrame = 0;  
static inline bool drawHitboxes = false;
float maxSpeed = 100.0f;  
const float SEE_AHEAD = 140.0f;  
const float MAX_FORCE = 80.0f;  
const float AVOID_FORCE = 150.0f;  
float damageCooldown = 0.0f;  
const float damageCooldownTime = 2.0f;  
bool isInvincible = false;  

Tmpl8::vec2 position;   
Tmpl8::vec2 velocity;  
Tmpl8::vec2 size;   
Rectangle hitbox;  

//ANIMATION  
float animationTimer = 0.0f;  
float animationSpeed = 0.2f;  
int currentFrame = 0;  
};