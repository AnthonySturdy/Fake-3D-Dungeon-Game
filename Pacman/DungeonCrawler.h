#pragma once
// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr

//I've disabled this to allow user to chose between pacman and dungeon crawler when the program launches.

//#ifdef WIN32 
//	#ifndef _DEBUG
//		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//	#endif
//#endif

#include<iostream>

// Just need to include main header file
#include "S2D/S2D.h"
#include "Entity.h"
#include "Player.h"
#include "DepthDrawOrder.h"
#include "LevelLoader.h"
#include "Camera.h"
#include "TextRenderer.h"
//#include "EnemySpawner.h"
//#include "Enemy.h"
//#include"Leaderboard.h"

class Enemy;
class EnemySpawner;
class Leaderboard;

using namespace S2D;

class DungeonCrawler : public Game {
private:
	std::vector<Entity*> levelData;
	Camera* camera;

	int frameCount = 0;

	//Main Menu
	Texture2D* mainMenuLogo;

	//Health UI
	Rect* healthHeartsSourceRect;
	Texture2D* healthHeartsImage;

	//Game Over
	Texture2D* gameOverImage;
	std::vector<std::string> leaderboardText;
	std::string usernameInput;
	bool isKeyboardPressed = false;
	bool isBackspacePressed = false;
	int leaderboardUpdateCount = 0;

public:
	static Player* _playerObject;
	static DepthDrawOrder* drawOrder;
	static vector<Enemy*> enemies;
	static EnemySpawner* enemySpawner;
	static Leaderboard* leaderboard;

	//Footstep		https://freesound.org/people/carrigsound/sounds/32637/
	static SoundEffect* footstepSound;
	//Attack		https://freesound.org/people/Black%20Snow/sounds/109413/
	static SoundEffect* playerAttackSound;
	//Enemy Die		https://freesound.org/people/JustInvoke/sounds/446115/
	static SoundEffect* enemyDieSound;
	//Damage		https://freesound.org/people/Dirtjm/sounds/262279/
	static SoundEffect* playerDamageSound;
	//Enemy Spawn	https://freesound.org/people/audiolarx/sounds/263946/
	static SoundEffect* enemySpawnSound;

	enum GameState {
		MAIN_MENU = 0,
		GAMEPLAY = 1,
		PAUSED = 2,
		GAME_OVER = 3
	};
	GameState curGameState = MAIN_MENU;

	void Main_Menu_Update(int elapsedTime);
	void Main_Menu_Draw(int elapsedTime);
	void Gameplay_Update(int elapsedTime);
	void Gameplay_Draw(int elapsedTime);
	void Paused_Update(int elapsedTime);
	void Paused_Draw(int elapsedTime);
	void Game_Over_Update(int elapsedTime);
	void Game_Over_Draw(int elapsedTime);

	char CurrentCharPressed(Input::Keys key);

	/// <summary> Constructs the Pacman class. </summary>
	DungeonCrawler(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~DungeonCrawler();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

};