#pragma once

#include <SDL.h>

class GameObject {
public:
	GameObject(SDL_Renderer* renderer, SDL_Texture* texture);
	~GameObject();

	// TODO: Collision?

	// 
	void tick();

	// Display game object to renderer
	void render();

	// Move the GameObject to new position, lerped by speed
	void move(float posX, float posY, float speed);

private:
	float mPosX = 0, mPosY = 0;
	float mVelX = 0, mVelY = 0;
	
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};
