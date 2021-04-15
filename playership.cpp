#include "playership.h"

#include "gameobject.h"
#include "texturewrapper.h"
#include "util.h"

PlayerShip::PlayerShip(TextureWrapper* textureWrapper) : GameObject(textureWrapper, 32, 32) {
	setMaxSpeed(0.5);
	turnSpeed = 0.4;
	accelerationPerSecond = 0.0005;
}

void PlayerShip::tick(float delta) {
	if (accelerating || decelerating) {
		float x, y;
		util::coordFromAngle(getRotation(), x, y);

		float accelerationX = x * accelerationPerSecond * delta;
		float accelerationY = y * accelerationPerSecond * delta;

		if (accelerating) {
			addVelocity(accelerationX, accelerationY);
		}
		else if (decelerating) {
			reduceVelocity(accelerationX, accelerationY);
		}
	}
	GameObject::tick(delta);
}

void PlayerShip::accelerate(bool toggle) {
	accelerating = toggle;
}

void PlayerShip::decelerate(bool toggle) {
	decelerating = toggle;
}

// DECELERATE, use gameobject decelerate method so you can stop
// OR, just dont allow backward thrust

void PlayerShip::turn(bool toggle, bool clockwise) {
	if (toggle) {
		float amt = clockwise ? turnSpeed : -turnSpeed;
		setAngularMomentum(amt);
	}
	else {
		setAngularMomentum(0);
	}
}
