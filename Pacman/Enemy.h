#pragma once
#include"Entity.h"
#include"DungeonCrawler.h"
#include"Player.h"

class Enemy : public Entity {
private:
	Player* target;

	float movementSpeed = 40.0f;
	bool isAttacking = false;
	float attackDelay = 0.8f;
	float currentAttackTime = 0.0f;

public:
	bool isDead = false;

	Enemy(std::string pModelName, int pModelHeight, int pFrameCount, Vector2 pInitialPos, float pInitialRotation);
	~Enemy();

	void AI(float elapsedTime);
	void Attack(float elapsedTime);
	void TakeDamage();

	Vector2 CalculateWalkDirection();
};