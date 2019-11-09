#include"EnemySpawner.h"
#include <time.h>


EnemySpawner::EnemySpawner() {
	enemySpawners = LevelLoader::GetEnemySpawners();
	srand(time(NULL));

	curSpawnDelay = fmodf((float)rand(), (spawnRateMax - spawnRateMin)) + spawnRateMin;
}

EnemySpawner::~EnemySpawner() {
	for (int i = 0; i < enemySpawners.size(); i++) {
		delete enemySpawners.at(i);
	}
}

void EnemySpawner::SpawnerTick(int elapsedTime) {
	curSpawnDelay -= elapsedTime / 1000.0f;
	if (curSpawnDelay <= 0.0f) {
		curSpawnDelay = fmod(rand(), (spawnRateMax - spawnRateMin)) + spawnRateMin;

		Vector2 spawnVec = *enemySpawners.at(rand() % enemySpawners.size());
		DungeonCrawler::enemies.push_back(new Enemy("slime", 20, 4, spawnVec, 0));

		Audio::Play(DungeonCrawler::enemySpawnSound);
	}
}