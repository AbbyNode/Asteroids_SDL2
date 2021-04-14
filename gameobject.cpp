#include "gameobject.h"

#include <algorithm>
#include "util.h"

GameObject::GameObject(TextureWrapper* textureWrapper, int width, int height)
	: textureWrapper(textureWrapper), width(width), height(height) {
}

GameObject::~GameObject() {}

void GameObject::tick(float delta) {
	float moveX = velX * delta;
	float moveY = velY * delta;

	// Collisions?

	posX += moveX;
	posY += moveY;
}

void GameObject::render() {
	SDL_Rect dest = { (int)posX, (int)posY, width, height };
	textureWrapper->render(&dest, rotation);
}

void GameObject::addVelocity(float x, float y) {
	float newVelX = velX + x;
	float newVelY = velY + y;
	
	// Cap to max speed
	float newSpeed = util::speed(newVelX, newVelY);
	if (newSpeed > maxSpeed) {
		float speedRatio = maxSpeed / newSpeed;
		newVelX *= speedRatio;
		newVelY *= speedRatio;
	}

	velX = newVelX;
	velY = newVelY;
}

void GameObject::reduceVelocity(float x, float y) {
	using std::min;

	float newVelX = min((velX - x), 0.0f);
	float newVelY = min((velY - y), 0.0f);

	velX = newVelX;
	velY = newVelY;
}

float GameObject::getSpeed() {
	return util::speed(velX, velY);
}
