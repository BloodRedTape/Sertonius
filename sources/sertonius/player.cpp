#include "sertonius/player.hpp"
#include <core/os/keyboard.hpp>
#include <core/print.hpp>
#include <core/os/mouse.hpp>
#include <core/math/transform.hpp>
#include <core/math/functions.hpp>
#include <imgui/widgets.hpp>

Player::Player(){

	Camera camera;
	camera.FOV  = 90;
	camera.Near = 0.1;
	camera.Far  = 20;
	camera.Aspect = 16 / 9.f;

	AddComponent<CameraComponent>(camera);

	//Position.y = -5;
}

void Player::Tick(float dt){
#if 1
	Vector3f direction;
	if (Keyboard::IsKeyPressed(Key::D))
		direction.x = 1.f;
	if (Keyboard::IsKeyPressed(Key::A))
		direction.x = -1.f;
	if (Keyboard::IsKeyPressed(Key::W))
		direction.y = 1.f;
	if (Keyboard::IsKeyPressed(Key::S))
		direction.y = -1.f;
	if (Keyboard::IsKeyPressed(Key::Space))
		direction.z = 1.f;
	if (Keyboard::IsKeyPressed(Key::LeftShift))
		direction.z = -1.f;
#if 1
	Position += (Math::Rotate<float>(Math::Rad(Vector3f{ 0, 0, Rotation.z})) * Vector4f(direction, 1.f)).XYZ() * dt;
#else
	Position += direction * dt;
#endif
#endif
}

void Player::OnMouseMove(Vector2s offset){
	//Println("Offset: %", offset);
	Rotation.x -=  offset.y;
	Rotation.x = Math::Clamp(Rotation.x, -90.f, 90.f);
	Rotation.z -=  offset.x;
}

void Player::OnMouseButtonPress(Mouse::Button button, Vector2s position){
	//Println("Button: %, pos: %", (int)button, position);
}
