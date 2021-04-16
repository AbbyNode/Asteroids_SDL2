#pragma once

#include <SDL.h>

class TextureWrapper {
public:
	static SDL_Renderer* renderer;

	TextureWrapper(SDL_Texture* texture, SDL_Rect* textureClip = nullptr);
	~TextureWrapper();

	void render(SDL_Rect* dest, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	SDL_Texture* texture = nullptr;
	SDL_Rect* textureClip = nullptr;
};
