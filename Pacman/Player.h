#pragma once
#include"Entity.h"
#include"Camera.h"

class Player : public Entity {
private:
	float speed = 0.12f;
	float direction = 0.3f;
	Camera* _camera;

	int playerRunTime = 0;

public:
	Player(std::string modelName, int modelHeight, int frameCount, Vector2 initialPos, float initialRotation, Camera* camera);
	~Player();

	int score = 0;
	int health = 6;


	void TakeDamage(int dmg);
	void Attack();

	void PlayerInput(Input::KeyboardState *keyState, float elapsedTime);
	bool IsColliding(int curTile);

	float GetDirection();

	float VectorDot(Vector2 a, Vector2 b);
	float AngleBetweenVectors(Vector2 a, Vector2 b, float a_mag, float b_mag);
};