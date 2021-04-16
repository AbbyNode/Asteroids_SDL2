#pragma once

#include "texturewrapper.h"

#include <vector>

class GameObject {
public:
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

	GameObject(TextureWrapper* textureWrapper, int width, int height, float collisionSizeRatio = 1);
	
	// 
	virtual void tick(float delta);

	// Display game object to renderer
	void render();

	void addVelocity(float x, float y);
	void reduceVelocity(float x, float y);
	void setAngularMomentum(float amt);

	void setMaxSpeed(float maxSpeed);

	void setCollidableGameObjects(std::vector<GameObject*>* collidableGameObjects);

	void getPosition(float& x, float& y);
	float getRotation();
	float getSpeed();

protected:
	void setPosition(float x, float y);
	virtual void collisionCallback(GameObject* gameObject);

private:
	TextureWrapper* textureWrapper = nullptr;

	int width, height;
	float collisionSize;

	float posX = 0, posY = 0;
	float rotation = 0;

	float velX = 0, velY = 0;
	float angularMomentum = 0;

	float maxSpeed = 10000;

	std::vector<GameObject*>* collidableGameObjects;

	bool isColliding(GameObject* gameObject);
};
