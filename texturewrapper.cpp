#include "texturewrapper.h"

SDL_Renderer* TextureWrapper::renderer = NULL;

TextureWrapper::TextureWrapper(SDL_Texture* texture, SDL_Rect* textureClip, int width, int height) {
	this->texture = texture;
	this->textureClip = textureClip;
	this->width = width;
	this->height = height;
}

TextureWrapper::~TextureWrapper() {}

void TextureWrapper::render(SDL_Rect* dest, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(renderer, texture, textureClip, dest, angle, center, flip);
}
