#include"LevelLoader.h"
#include<iostream>
#include<math.h>
#include<sstream>

#define NUM_OF_BLOCK_TYPES 11
#define LEVEL_SIZE 10

const std::string LevelLoader::levelBlockInformation[NUM_OF_BLOCK_TYPES] = {
	"floor",
	"pillar",
	"w_wall",
	"n_wall",
	"e_wall",
	"s_wall",
	"ne_corner",
	"nw_corner",
	"se_corner",
	"sw_corner",
	"enemy_spawner" };

const int LevelLoader::levelBlockHeights[NUM_OF_BLOCK_TYPES] = {
	2,
	40,
	40,
	40,
	40,
	40,
	40,
	40,
	40,
	40, 
	3};

std::vector<Entity*> LevelLoader::LoadLevel() {
	//Load level data from file
	std::ifstream levelFile("DungeonCrawler_Assets/Level_Data.txt");

	//Vectors for holding data.
	std::vector<std::string> levelStringData(LEVEL_SIZE, "");
	std::vector<Entity*> levelData;

	if (levelFile.is_open()) {
		std::string line;
		int curLine = 0;
		while (std::getline(levelFile, line)) {
			levelStringData[curLine] = line;
			curLine++;
		}

		levelFile.close();
	}

	//Create entity objects from level information
	for (int x = 0; x < LEVEL_SIZE; x++) {
		for (int y = 0; y < LEVEL_SIZE; y++) {
			float nx = x * 40;
			float ny = y * 40;
			levelData.push_back(new Entity(levelBlockInformation[CharToInt(levelStringData[y][x])], levelBlockHeights[CharToInt(levelStringData[y][x])], 1,Vector2(nx, ny), 0, true));
		}
	}

	return levelData;
}

int LevelLoader::CharToInt(char chr) {
	int val;

	std::stringstream ss;
	ss << chr;
	ss >> std::hex >> val;

	return val;
}

int LevelLoader::GetLevelTile(S2D::Vector2* vec) {
	Vector2 v(floor(vec->X), floor(vec->Y));
	std::stringstream ss;
	std::string str;

	std::ifstream levelFile("DungeonCrawler_Assets/Level_Data.txt");
	if (levelFile.is_open()) {
		ss << levelFile.rdbuf();	//Read file into stringstream
		str = ss.str();				//Put stringstream contents into string
		str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());	//Remove linebreaks from string

		levelFile.close();
	}

	return CharToInt(str[v.Y * LEVEL_SIZE + v.X]);
}

std::vector<Vector2*> LevelLoader::GetPillars() {
	//THIS IS SAME CODE FROM LoadLevel(). MODIFIED TO RETURN ONLY PILLARS.
	//Load level data from file
	std::ifstream levelFile("DungeonCrawler_Assets/Level_Data.txt");

	//Vectors for holding data.
	std::vector<std::string> levelStringData(LEVEL_SIZE, "");
	std::vector<Vector2*> pillars;

	if (levelFile.is_open()) {
		std::string line;
		int curLine = 0;
		while (std::getline(levelFile, line)) {
			levelStringData[curLine] = line;
			curLine++;
		}

		levelFile.close();
	}

	//Create entity objects from level information
	for (int x = 0; x < LEVEL_SIZE; x++) {
		for (int y = 0; y < LEVEL_SIZE; y++) {
			float nx = x * 40;
			float ny = y * 40;
			if(CharToInt(levelStringData[y][x]) == 1)
				pillars.push_back(new Vector2(nx, ny));
		}
	}

	return pillars;
}

std::vector<Vector2*> LevelLoader::GetEnemySpawners() {
	//THIS IS SAME CODE FROM LoadLevel(). MODIFIED TO RETURN ONLY PILLARS.
	//Load level data from file
	std::ifstream levelFile("DungeonCrawler_Assets/Level_Data.txt");

	//Vectors for holding data.
	std::vector<std::string> levelStringData(LEVEL_SIZE, "");
	std::vector<Vector2*> spawners;

	if (levelFile.is_open()) {
		std::string line;
		int curLine = 0;
		while (std::getline(levelFile, line)) {
			levelStringData[curLine] = line;
			curLine++;
		}

		levelFile.close();
	}

	//Create entity objects from level information
	for (int x = 0; x < LEVEL_SIZE; x++) {
		for (int y = 0; y < LEVEL_SIZE; y++) {
			float nx = x * 40;
			float ny = y * 40;
			if (CharToInt(levelStringData[y][x]) == 10)
				spawners.push_back(new Vector2(nx, ny));
		}
	}

	return spawners;
}