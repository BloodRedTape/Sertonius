#include "sertonius/player.hpp"
#include <core/os/keyboard.hpp>
#include <core/print.hpp>

Player::Player(){

	Camera camera;
	camera.FOV  = 90;
	camera.Near = 0;
	camera.Far  = 100;

	AddComponent<CameraComponent>(camera);
}

void Player::Tick(float dt){
	if (Keyboard::IsKeyPressed(Key::D))
		Position.x += dt;
	if (Keyboard::IsKeyPressed(Key::A))
		Position.x -= dt;
}
