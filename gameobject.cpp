#include "gameobject.h"

#include <algorithm>
#include <vector>

#include "texturewrapper.h"
#include "util.h"

int GameObject::SCREEN_HEIGHT = 0;
int GameObject::SCREEN_WIDTH = 0;

GameObject::GameObject(TextureWrapper* textureWrapper, int width, int height, float collisionSizeRatio)
	: textureWrapper(textureWrapper), width(width), height(height) {

	collisionSize = (width >= height ? width : height) * collisionSizeRatio;
}

void GameObject::tick(float delta) {
	// Movement
	float newPosX = posX + (velX * delta);
	float newPosY = posY + (velY * delta);
	posX = util::warpValue(newPosX, -width, SCREEN_WIDTH);;
	posY = util::warpValue(newPosY, -height, SCREEN_HEIGHT);;

	// Rotation
	float newRotation = rotation + (angularMomentum * delta);
	rotation = util::warpAngle(newRotation);

	// Collisions
	if (collidableGameObjects != nullptr) {
		for (GameObject* gameObject : *collidableGameObjects) {
			if (isColliding(gameObject)) {
				collisionCallback(gameObject);
			}
		}
	}
}

void GameObject::render() {
	SDL_Rect dest = { (int)posX, (int)posY, width, height };
	textureWrapper->render(&dest, rotation);
}

void GameObject::addVelocity(float x, float y) {
	float newVelX = velX + x;
	float newVelY = velY + y;
	
	// Cap to max speed
	float newSpeed = util::hypotenuse(newVelX, newVelY);
	if (newSpeed > maxSpeed) {
		float speedRatio = maxSpeed / newSpeed;
		newVelX *= speedRatio;
		newVelY *= speedRatio;
	}

	velX = newVelX;
	velY = newVelY;
}

void GameObject::reduceVelocity(float x, float y) {
	velX = util::toZero(velX, -x);
	velY = util::toZero(velY, -y);
}

void GameObject::setAngularMomentum(float angularMomentum) {
	this->angularMomentum = angularMomentum;
}

void GameObject::setMaxSpeed(float maxSpeed) {
	this->maxSpeed = (maxSpeed >= 0) ? maxSpeed : 0;
}

void GameObject::setCollidableGameObjects(std::vector<GameObject*>* collidableGameObjects) {
	this->collidableGameObjects = collidableGameObjects;
}

void GameObject::getPosition(float& x, float& y) {
	x = posX;
	y = posY;
}

float GameObject::getRotation() {
	return rotation;
}

float GameObject::getSpeed() {
	return util::hypotenuse(velX, velY);
}

//

void GameObject::setPosition(float x, float y) {
	posX = x;
	posY = y;
}

void GameObject::collisionCallback(GameObject* gameObject) {}

//

bool GameObject::isColliding(GameObject* gameObject) {
	float distanceBetweenCenters = util::distance(posX, posY, gameObject->posX, gameObject->posY);
	float largestCollisionDistance = collisionSize + gameObject->collisionSize;
	return distanceBetweenCenters <= largestCollisionDistance;
}
