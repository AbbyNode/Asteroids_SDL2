#include "bullet.h"

Bullet::Bullet(TextureWrapper* textureWrapper, float posX, float posY, float velX, float velY)
	: GameObject(textureWrapper, 64, 64) {
	setPosition(posX, posY);
	addVelocity(velX, velY);
}

void Bullet::collisionCallback(GameObject* gameObject) {

}
