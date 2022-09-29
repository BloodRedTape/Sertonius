#include "sertonius/player.hpp"
#include <core/os/keyboard.hpp>
#include <core/print.hpp>
#include <core/os/mouse.hpp>
#include <core/math/transform.hpp>
#include <imgui/widgets.hpp>

Player::Player(){

	Camera camera;
	camera.FOV  = 90;
	camera.Near = 0;
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
	Position += (Math::Rotate<float>(Math::Rad(Rotation)) * Vector4f(direction, 1.f)).XYZ() * dt;
#else
	Position += direction * dt;
#endif
#endif

#if 1
	static Vector2s mouse_position = Mouse::GlobalPosition();
	
	Vector2s new_position = Mouse::GlobalPosition();
	Vector2s offset = new_position - mouse_position;
	Rotation.x +=  offset.y;
	Rotation.z +=  offset.x;
	mouse_position = new_position;
#endif
}
