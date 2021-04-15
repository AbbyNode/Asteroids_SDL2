#include "playership.h"

#include "gameobject.h"
#include "texturewrapper.h"
#include "util.h"

PlayerShip::PlayerShip(TextureWrapper* textureWrapper) : GameObject(textureWrapper, 32, 32) {
	setMaxSpeed(1);
}

void PlayerShip::tick(float delta) {
	if (accelerationX != 0 || accelerationY != 0) {
		addVelocity(accelerationX * delta, accelerationY * delta);
	}
	GameObject::tick(delta);
}

void PlayerShip::accelerate(bool toggle, bool forward) {
	if (toggle) {
		float amt = forward ? accelerationPerSecond : -accelerationPerSecond;

		float x, y;
		util::coordFromAngle(getRotation(), x, y);
		//printf("%f | %f | %f\n", getRotation(), x, y);

		accelerationX = amt * x;
		accelerationY = amt * y;
	}
	else {
		accelerationX = 0;
		accelerationY = 0;
	}
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
