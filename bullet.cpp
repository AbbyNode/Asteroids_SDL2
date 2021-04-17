#include "bullet.h"

Bullet::Bullet(TextureWrapper* textureWrapper, float posX, float posY, float velX, float velY
	, std::function<void(GameObject* thisBullet)> destroyCallback)
	: GameObject(textureWrapper, 64, 64, 0.1f), destroyCallback(destroyCallback) {
	setPosition(posX, posY);
	addVelocity(velX, velY);
}

void Bullet::tick(float delta) {
	GameObject::tick(delta);

	float x, y;
	getPosition(x, y);
	if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
		destroyCallback(this);
	}
}
