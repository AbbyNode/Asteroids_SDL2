#pragma once

#include "texturewrapper.h"

class GameObject {
public:
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

	GameObject(TextureWrapper* textureWrapper, int width, int height);
	~GameObject();

	// 
	virtual void tick(float delta);

	// Display game object to renderer
	void render();

	void addVelocity(float x, float y);
	void reduceVelocity(float x, float y);

	void setAngularMomentum(float amt);

	void setMaxSpeed(float maxSpeed);

	float getRotation();
	float getSpeed();

private:
	int width, height;

	float posX = 0, posY = 0;
	float rotation = 0;

	float velX = 0, velY = 0;
	float angularMomentum = 0;

	float maxSpeed = 10000;

	TextureWrapper* textureWrapper = NULL;
};
