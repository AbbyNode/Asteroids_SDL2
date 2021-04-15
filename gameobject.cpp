#include "gameobject.h"

#include <algorithm>

#include "texturewrapper.h"
#include "util.h"

int GameObject::SCREEN_HEIGHT = 0;
int GameObject::SCREEN_WIDTH = 0;

GameObject::GameObject(TextureWrapper* textureWrapper, int width, int height)
	: textureWrapper(textureWrapper), width(width), height(height) {
}

GameObject::~GameObject() {}

void GameObject::tick(float delta) {
	float moveX = velX * delta;
	float moveY = velY * delta;

	float newRotation = rotation + (angularMomentum * delta);
	rotation = util::warpAngle(newRotation);

	// Collisions?

	float newPosX = posX + moveX;
	float newPosY = posY + moveY;
	newPosX = util::warpValue(newPosX, 0, SCREEN_WIDTH);
	newPosY = util::warpValue(newPosY, 0, SCREEN_HEIGHT);

	posX = newPosX;
	posY = newPosY;
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
	/*
	using std::max;
	using std::min;

	float newVelX = velX - x; // = max((velX - x), 0.0f);
	float newVelY = velY - y; // = max((velY - y), 0.0f);

	// Stop at 0
	if (velX > 0) {
		newVelX -= min(x, newVelX);
	}
	else if (velX < 0) {
		newVelX -= max(x, newVelX);
	}

	if (velY > 0) {
		newVelY -= min(y, newVelY);
	}
	else if (velY < 0) {

	}
	*/

	velX = util::toZero(velX, -x);
	velY = util::toZero(velY, -y);
}

void GameObject::setAngularMomentum(float angularMomentum) {
	this->angularMomentum = angularMomentum;
}

void GameObject::setMaxSpeed(float maxSpeed) {
	this->maxSpeed = (maxSpeed >= 0) ? maxSpeed : 0;
}

float GameObject::getRotation() {
	return rotation;
}

float GameObject::getSpeed() {
	return util::speed(velX, velY);
}