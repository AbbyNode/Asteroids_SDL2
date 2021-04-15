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
	rotation = util::simplifyAngle(newRotation);

	// Collisions?

	float newPosX = posX + moveX;
	float newPosY = posY + moveY;

	if (newPosX > SCREEN_WIDTH) {
		newPosX -= SCREEN_WIDTH;
	}
	else if (newPosX < 0) {
		newPosX += SCREEN_WIDTH;
	}

	if (newPosY > SCREEN_HEIGHT) {
		newPosY -= SCREEN_HEIGHT;
	}
	else if (newPosY < 0) {

	}

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