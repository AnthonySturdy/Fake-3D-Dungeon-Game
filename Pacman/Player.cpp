#include "Player.h"
#include "DungeonCrawler.h"
#include "LevelLoader.h"
#include "Enemy.h"
#include<iostream>

Player::Player(std::string pModelName, int pModelHeight, int pFrameCount, Vector2 pInitialPos, float pInitialRotation, Camera* camera) : Entity(pModelName, pModelHeight, pFrameCount, pInitialPos, pInitialRotation, false) {
	direction = pInitialRotation;
	_camera = camera;
}

Player::~Player() {

}

//Handles Player Movement Input
void Player::PlayerInput(Input::KeyboardState *keyState, float elapsedTime) {
	//Control Player
	float dir;
	Vector2 rotationVec(0, 0);

	if (keyState->IsKeyDown(Input::Keys::D)) {
		rotationVec.X = -1;
	} else if (keyState->IsKeyDown(Input::Keys::A)) {
		rotationVec.X = 1;
	}

	if (keyState->IsKeyDown(Input::Keys::W)) {
		rotationVec.Y = -1;
	} else if (keyState->IsKeyDown(Input::Keys::S)) {
		rotationVec.Y = 1;
	}

	//Attack Input
	if (keyState->IsKeyDown(Input::Keys::SPACE) && GetCurrentAnim() != "_attack") {
		ChangeAnimation("_attack", 4, false);
		Attack();
	}

	if (playerRunTime > 400) {
		Audio::Play(DungeonCrawler::footstepSound);
		playerRunTime = 0;
	}

	//If player is moving
	if (rotationVec != Vector2(0, 0) && GetCurrentAnim() != "_attack") {
		dir = atan2(rotationVec.X, rotationVec.Y);		//Converts vector to radians
		direction = dir * (180.0 / 3.141);

		//Apply camera rotation to player rotation
		float combinedDir = _camera->GetRotation() + direction + 90;
		combinedDir = fmod(combinedDir, 360);
		combinedDir = fmod(combinedDir, -360);

		float rad = combinedDir * 3.141 / 180;
		Vector2 finalVec(cos(rad), sin(rad));

		direction = -combinedDir + 90;

		playerRunTime += elapsedTime;

		//Change player position
		SetPosition(Vector2(GetPosition()->X + (finalVec.X * speed * elapsedTime), GetPosition()->Y + (finalVec.Y * speed * elapsedTime)));

		//Collision Detection
		if (IsColliding(LevelLoader::GetLevelTile(&Vector2(GetPosition()->X / 40, GetPosition()->Y / 40)))) {
			SetPosition(Vector2(GetPosition()->X - (finalVec.X * speed * elapsedTime), GetPosition()->Y - (finalVec.Y * speed * elapsedTime)));
		}

		//Pillar collision detection
		std::vector<Vector2*> pillarPositions = LevelLoader::GetPillars();

		for (int i = 0; i < pillarPositions.size(); i++) {
			const int offset = 20;
			Vector2 distVec((pillarPositions.at(i)->X + offset) - GetPosition()->X, (pillarPositions.at(i)->Y + offset) - GetPosition()->Y);
			float dist = sqrt((distVec.X * distVec.X) + (distVec.Y * distVec.Y));

			if(dist < 10)
				SetPosition(Vector2(GetPosition()->X - (finalVec.X * speed * elapsedTime), GetPosition()->Y - (finalVec.Y * speed * elapsedTime)));

			delete pillarPositions.at(i);
			pillarPositions.at(i) = nullptr;
		}

		//Change Anim to walk
		if (GetCurrentAnim() != "_walk") {
			ChangeAnimation("_walk", 4, true);
		}
	} else {	//If player isn't walking
		//Change Anim to idle
		if (GetCurrentAnim() == "_walk") {
			ChangeAnimation("", 1, true);
		}
	}

}

bool Player::IsColliding(int curTile) {
	switch (curTile) {
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		return true;
		break;

	default:
		return false;
		break;
	}
}

void Player::TakeDamage(int dmg) {
	health -= dmg;
	Audio::Play(DungeonCrawler::playerDamageSound);
}

void Player::Attack() {
	Audio::Play(DungeonCrawler::playerAttackSound);

	for (int i = 0; i < DungeonCrawler::enemies.size(); i++) {
		//Distance from player to enemy.
		Vector2 enemyToPlayerVec((GetPosition()->X) - (DungeonCrawler::enemies.at(i)->GetPosition()->X) - 10.0f, (GetPosition()->Y) - (DungeonCrawler::enemies.at(i)->GetPosition()->Y) - 10.0f);
		float enemyToPlayerDist = sqrt((enemyToPlayerVec.X * enemyToPlayerVec.X) + (enemyToPlayerVec.Y * enemyToPlayerVec.Y));
		Vector2 enemyToPlayerVecNorm(enemyToPlayerVec.X / enemyToPlayerDist, enemyToPlayerVec.Y / enemyToPlayerDist);

		float playerDirRad = (GetDirection() * 3.141f) / 180;
		Vector2 playerDirVecNorm((float)cos(playerDirRad), (float)sin(playerDirRad));
		float playerDirMag = sqrt((playerDirVecNorm.X * playerDirVecNorm.X) + (playerDirVecNorm.Y * playerDirVecNorm.Y));

		//If within hitting distance
		if (enemyToPlayerDist < 30.0f) {
			DungeonCrawler::enemies.at(i)->TakeDamage();
			score++;
		}
	}

}

float Player::GetDirection() {
	return direction;
}

float Player::VectorDot(Vector2 a, Vector2 b) {
	return a.X * b.X + a.Y * b.Y;
}