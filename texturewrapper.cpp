#include "texturewrapper.h"

SDL_Renderer* TextureWrapper::renderer = NULL;

TextureWrapper::TextureWrapper(SDL_Texture* texture, SDL_Rect* textureClip)
	: texture(texture), textureClip(textureClip) {}

TextureWrapper::~TextureWrapper() {

}

void TextureWrapper::render(SDL_Rect* dest, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(renderer, texture, textureClip, dest, angle, center, flip);
}
