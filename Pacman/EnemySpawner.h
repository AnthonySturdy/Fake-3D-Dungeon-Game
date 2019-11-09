#pragma once
#include<vector>
#include"S2D/S2D.h"
#include"LevelLoader.h"
#include"DungeonCrawler.h"
#include"Enemy.h"

using namespace S2D;

class EnemySpawner {
private:
	std::vector<Vector2*> enemySpawners;

	const float spawnRateMin = 0.5f;
	const float spawnRateMax = 4.0f;
	float curSpawnDelay = 0.0f;

public:
	EnemySpawner();
	~EnemySpawner();

	void SpawnerTick(int elapsedTime);
};