#include"Enemy.h"
#include"TextRenderer.h"

Enemy::Enemy(std::string ModelName, int ModelHeight, int FrameCount, Vector2 InitialPos, float InitialRotation) : Entity(ModelName, ModelHeight, FrameCount, InitialPos, InitialRotation, true) {

}

Enemy::~Enemy() {

}

void Enemy::AI(float elapsedTime) {
	if (isDead) {
		for (int i = 0; i < DungeonCrawler::enemies.size(); i++) {
			if (DungeonCrawler::enemies.at(i) == this) {
				DungeonCrawler::enemies.erase(DungeonCrawler::enemies.begin() + i);
				delete this;
				break;
			}
		}
	} else {

		//Calculate distance between enemy and player
		const float offset = 10.5f;
		Vector2 distVec(DungeonCrawler::_playerObject->GetPosition()->X - GetPosition()->X - offset, DungeonCrawler::_playerObject->GetPosition()->Y - GetPosition()->Y - offset);
		float dist = sqrt((distVec.X * distVec.X) + (distVec.Y * distVec.Y));

		//Movement
		if (dist > 15)
			SetPosition(Vector2(GetPosition()->X + (CalculateWalkDirection().X * movementSpeed * (elapsedTime / 1000.0f)), GetPosition()->Y + (CalculateWalkDirection().Y * movementSpeed * (elapsedTime / 1000.0f))));

		//Attack
		if (dist < 15.5f)
			Attack(elapsedTime);
	}
}

void Enemy::Attack(float elapsedTime) {
	currentAttackTime += elapsedTime / 1000.0f;
	if (currentAttackTime > attackDelay) {
		currentAttackTime = 0.0f;
		DungeonCrawler::_playerObject->TakeDamage(1);
	}
}

void Enemy::TakeDamage() {
	Audio::Play(DungeonCrawler::enemyDieSound);
	isDead = true;
}

Vector2 Enemy::CalculateWalkDirection() {
	const float offset = 10.5f;
	float dx = DungeonCrawler::_playerObject->GetPosition()->X - GetPosition()->X - offset;
	float dy = DungeonCrawler::_playerObject->GetPosition()->Y - GetPosition()->Y - offset;
	float mag = sqrt((dx * dx) + (dy * dy));

	return Vector2(dx / mag, dy / mag);
}