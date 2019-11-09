#pragma once
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include"Entity.h"
#include"S2D/S2D.h"

class LevelLoader {

public:
	static std::vector<Entity*> LoadLevel();

	static int CharToInt(char str);
	static int GetLevelTile(S2D::Vector2* vec);
	static std::vector<Vector2*> GetPillars();
	static std::vector<Vector2*> GetEnemySpawners();

	const static std::string levelBlockInformation[];
	const static int levelBlockHeights[];
};