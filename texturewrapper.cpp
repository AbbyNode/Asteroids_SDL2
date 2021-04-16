#include "texturewrapper.h"

#include <SDL.h>

SDL_Renderer* TextureWrapper::renderer = nullptr;

TextureWrapper::TextureWrapper(SDL_Texture* texture, SDL_Rect* textureClip)
	: texture(texture), textureClip(textureClip) {}

TextureWrapper::~TextureWrapper() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
	textureClip = nullptr;
}

void TextureWrapper::render(SDL_Rect* dest, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(renderer, texture, textureClip, dest, angle, center, flip);
}
