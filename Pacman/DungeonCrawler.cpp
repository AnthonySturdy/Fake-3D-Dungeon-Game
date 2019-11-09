#include"DungeonCrawler.h"
#include<vector>
#include<string>
#include<sstream>
#include<cmath>

#include"Enemy.h"
#include"EnemySpawner.h"
#include"Leaderboard.h"

//Static Variables
Player* DungeonCrawler::_playerObject;
DepthDrawOrder* DungeonCrawler::drawOrder;
vector<Enemy*> DungeonCrawler::enemies;
EnemySpawner* DungeonCrawler::enemySpawner;
Leaderboard* DungeonCrawler::leaderboard;

SoundEffect* DungeonCrawler::footstepSound;
SoundEffect* DungeonCrawler::playerAttackSound;
SoundEffect* DungeonCrawler::enemyDieSound;
SoundEffect* DungeonCrawler::playerDamageSound;
SoundEffect* DungeonCrawler::enemySpawnSound;

/*
FIX:
	-Player can attack enemy when not looking at them
*/

DungeonCrawler::DungeonCrawler(int argc, char* argv[]) : Game(argc, argv) {
	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1280, 720, false, 25, 25, "Dungeon Ninja", 30);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

DungeonCrawler::~DungeonCrawler() {
	for (int i = 0; i < levelData.size(); i++) {
		delete levelData.at(i);
	}
	levelData.clear();

	delete _playerObject;
	_playerObject = nullptr;

	delete drawOrder;
	drawOrder = nullptr;

	for (int i = 0; i < enemies.size(); i++) {
		delete enemies.at(i);
	}
	enemies.clear();

	delete enemySpawner;

	delete mainMenuLogo;

	delete gameOverImage;

	delete footstepSound;
	delete playerAttackSound;
	delete playerDamageSound;
	delete enemyDieSound;
	delete enemySpawnSound;
}

void DungeonCrawler::LoadContent() {
	//Creates Camera
	camera = new Camera();

	//Creates Draw Order
	drawOrder = new DepthDrawOrder();

	//Creates Player
	_playerObject = new Player("player", 20, 1, Vector2(200, 200), 0, camera);

	//Pre-load Player animations
	_playerObject->ChangeAnimation("_attack", 4, false);
	_playerObject->ChangeAnimation("_walk", 4, true);
	_playerObject->ChangeAnimation("", 1, true);

	//Create Enemy Spawner
	enemySpawner = new EnemySpawner();

	//Load Level data from .txt file to Entity objects
	levelData = LevelLoader::LoadLevel();

	//Initialise Leadboards
	leaderboard = new Leaderboard();

	//Main Menu
	mainMenuLogo = new Texture2D();
	mainMenuLogo->Load("DungeonCrawler_Assets/Logo.png", false);

	//Health UI
	healthHeartsImage = new Texture2D();
	healthHeartsImage->Load("DungeonCrawler_Assets/Hearts.png", false);
	healthHeartsSourceRect = new Rect(0, 0, healthHeartsImage->GetWidth(), healthHeartsImage->GetHeight());

	//Game Over
	gameOverImage = new Texture2D();
	gameOverImage->Load("DungeonCrawler_Assets/Game Over.png", false);

	//Load Sound Effects
	footstepSound = new SoundEffect();
	footstepSound->Load("DungeonCrawler_Assets/Audio/Footstep.wav");

	playerAttackSound = new SoundEffect();
	playerAttackSound->Load("DungeonCrawler_Assets/Audio/PlayerAttack.wav");

	playerDamageSound = new SoundEffect();
	playerDamageSound->Load("DungeonCrawler_Assets/Audio/PlayerDamage.wav");

	enemyDieSound = new SoundEffect();
	enemyDieSound->Load("DungeonCrawler_Assets/Audio/EnemyDie.wav");

	enemySpawnSound = new SoundEffect();
	enemySpawnSound->Load("DungeonCrawler_Assets/Audio/EnemySpawn.wav");

}

void DungeonCrawler::Update(int elapsedTime) {
	switch (curGameState) {
	case MAIN_MENU:
		Main_Menu_Update(elapsedTime);
		break;

	case GAMEPLAY:
		Gameplay_Update(elapsedTime);
		break;

	case PAUSED:
		Paused_Update(elapsedTime);
		break;

	case GAME_OVER:
		Game_Over_Update(elapsedTime);
		break;
	}
}

void DungeonCrawler::Draw(int elapsedTime) {
	SpriteBatch::BeginDraw();

	switch (curGameState) {
	case MAIN_MENU:
		Main_Menu_Draw(elapsedTime);
		break;

	case GAMEPLAY:
		Gameplay_Draw(elapsedTime);
		break;

	case PAUSED:
		Gameplay_Draw(elapsedTime);	//Draw paused gameplay under pause menu
		Paused_Draw(elapsedTime);
		break;

	case GAME_OVER:
		Game_Over_Draw(elapsedTime);
		break;
	}

	SpriteBatch::EndDraw();
}

void DungeonCrawler::Main_Menu_Update(int elapsedTime) {
	Input::KeyboardState *state = Input::Keyboard::GetState();

	if (state->IsKeyDown(Input::Keys::RETURN)) {
		curGameState = GAMEPLAY;
	}
}

void DungeonCrawler::Main_Menu_Draw(int elapsedTime) {
	SpriteBatch::Draw(mainMenuLogo, &Vector2(Graphics::GetViewportWidth() / 2 - mainMenuLogo->GetWidth() / 2, 125), 1.0f, 0.0f);
	TextRenderer::RenderText(&Vector2((Graphics::GetViewportWidth() / 2) - 95, 475), "Press ENTER to Play", Color(253/255.0f, 0, 23/255.0f), 1);
}

void DungeonCrawler::Gameplay_Update(int elapsedTime) {
	//Handle player death
	if (_playerObject->health <= 0) {
		leaderboardUpdateCount = 0;
		leaderboardText = leaderboard->GetFormattedScores();
		curGameState = GAME_OVER;
	}

	healthHeartsSourceRect->Width = _playerObject->health * 16;

	Input::KeyboardState *state = Input::Keyboard::GetState();

	if (state->IsKeyDown(Input::Keys::P)) {
		curGameState = PAUSED;
	}

	for (int i = 0; i < levelData.size(); i++) {
		levelData.at(i)->SetRotation(camera->GetRotation());
	}

	enemySpawner->SpawnerTick(elapsedTime);
	for (int i = 0; i < enemies.size(); i++) {
		enemies.at(i)->SetRotation(camera->GetRotation());
		enemies.at(i)->AI(elapsedTime);
	}

	_playerObject->PlayerInput(state, elapsedTime);
	_playerObject->SetRotation(_playerObject->GetDirection() + camera->GetRotation());

	camera->CameraControl(state, elapsedTime);

	frameCount++;
}

void DungeonCrawler::Gameplay_Draw(int elapsedTime) {
	//Set background colour
	Color* c = new Color(1 / 255.f, 1 / 255.f, 1 / 255.f);
	SpriteBatch::DrawRectangle(0, 0, Graphics::GetViewportWidth(), Graphics::GetViewportHeight(), c);
	delete c;
	c = nullptr;

	//Draw Floor & Set Rotation
	for (int i = 0; i < levelData.size(); i++) {
		levelData.at(i)->DrawEntity(elapsedTime);
	}

	//Draw Player & Set Rotation
	_playerObject->DrawEntity(elapsedTime);

	//Draw Enemies
	for (int i = 0; i < enemies.size(); i++) {
		enemies.at(i)->DrawEntity(elapsedTime);
	}

	//Get this here for draw string, as RenderAll clears obj list.
	int spriteCount = drawOrder->GetSpriteCount();

	drawOrder->RenderAll();
	
	//Draw Health
	SpriteBatch::Draw(healthHeartsImage, &Vector2(10, 10), healthHeartsSourceRect, &Vector2(0, 0), 1, 0, Color::White, SpriteEffect::NONE);

	//Draw Score
	std::stringstream ss;
	ss << "Score: " << _playerObject->score;
	TextRenderer::RenderText(&Vector2(10, 47), ss.str(), *Color::Red, 1);
}

void DungeonCrawler::Paused_Update(int elapsedTime) {
	Input::KeyboardState *state = Input::Keyboard::GetState();

	if (state->IsKeyDown(Input::Keys::P)) {
		curGameState = GAMEPLAY;
	}
}

void DungeonCrawler::Paused_Draw(int elapsedTime) {
	SpriteBatch::DrawRectangle(&Rect(0, 0, Graphics::GetViewportWidth(), Graphics::GetViewportHeight()), &Color(0, 0, 0, 0.5f));
	TextRenderer::RenderText(&Vector2(200, 200), "Paused", *Color::Cyan, 1);
}

void DungeonCrawler::Game_Over_Update(int elapsedTime) {
	Input::KeyboardState *state = Input::Keyboard::GetState();

	//Refresh Leaderboard every 25 seconds
	if (leaderboardUpdateCount > 25000) {
		leaderboardText = leaderboard->GetFormattedScores();
		leaderboardUpdateCount = 0;
	}

	//Typing username
	if (isKeyboardPressed == false) {
		for (int i = 0; i < state->GetPressedKeys().size(); i++) {
			if (CurrentCharPressed(state->GetPressedKeys().at(i)) != NULL && usernameInput.size() < 15) {
				usernameInput += CurrentCharPressed(state->GetPressedKeys().at(i));
				isKeyboardPressed = true;
			}
		}
	} else if (state->GetPressedKeys().size() == 0){
		isKeyboardPressed = false;
	}

	//Username backspace
	if (state->IsKeyDown(Input::Keys::BACKSPACE) && isBackspacePressed == false) {
		if (usernameInput.size() > 0) {
			usernameInput.pop_back();
		}
		isBackspacePressed = true;
	} else if(state->IsKeyUp(Input::Keys::BACKSPACE)) {
		isBackspacePressed = false;
	}


	if (state->IsKeyDown(Input::Keys::RETURN) && usernameInput.size() != 0) {
		leaderboard->PostScore(usernameInput, _playerObject->score);

		_playerObject->health = 6;
		_playerObject->score = 0;

		_playerObject->SetPosition(Vector2(200, 200));

		for (int i = 0; i < enemies.size(); i++) {
			delete enemies.at(i);
		}
		enemies.clear();

		curGameState = MAIN_MENU;
	}

	leaderboardUpdateCount += elapsedTime;
}

void DungeonCrawler::Game_Over_Draw(int elapsedTime) {
	SpriteBatch::Draw(gameOverImage, &Vector2(Graphics::GetViewportWidth() / 2 - gameOverImage->GetWidth() / 2, 125), 1.0f, 0.0f);

	for (int i = 0; i < leaderboardText.size(); i++) {
		TextRenderer::RenderText(&Vector2(350 + (i * 400), 400), leaderboardText.at(i), *Color::White, 1);
	}

	int userPlacement;
	std::vector<int> scores = leaderboard->GetUserScores();
	for (int i = 0; i < scores.size(); i++) {
		userPlacement = i + 1;
		if (scores.at(i) < _playerObject->score) {
			break;
		} else if (i == scores.size() - 1) {
			userPlacement++;
		}
	}

	std::stringstream ss;
	ss << std::to_string(userPlacement) << ". " << usernameInput << "<";
	TextRenderer::RenderText(&Vector2(750, 590), ss.str(), *Color::White, 1);
}

char DungeonCrawler::CurrentCharPressed(Input::Keys key) {
	switch (key) {
		case Input::Keys::A:
			return 'A';
			break;
		case Input::Keys::B:
			return 'B';
			break;
		case Input::Keys::C:
			return 'C';
			break;
		case Input::Keys::D:
			return 'D';
			break;
		case Input::Keys::E:
			return 'E';
			break;
		case Input::Keys::F:
			return 'F';
			break;
		case Input::Keys::G:
			return 'G';
			break;
		case Input::Keys::H:
			return 'H';
			break;
		case Input::Keys::I:
			return 'I';
			break;
		case Input::Keys::J:
			return 'J';
			break;
		case Input::Keys::K:
			return 'K';
			break;
		case Input::Keys::L:
			return 'L';
			break;
		case Input::Keys::M:
			return 'M';
			break;
		case Input::Keys::N:
			return 'N';
			break;
		case Input::Keys::O:
			return 'O';
			break;
		case Input::Keys::P:
			return 'P';
			break;
		case Input::Keys::Q:
			return 'Q';
			break;
		case Input::Keys::R:
			return 'R';
			break;
		case Input::Keys::S:
			return 'S';
			break;
		case Input::Keys::T:
			return 'T';
			break;
		case Input::Keys::U:
			return 'U';
			break;
		case Input::Keys::V:
			return 'V';
			break;
		case Input::Keys::W:
			return 'W';
			break;
		case Input::Keys::X:
			return 'X';
			break;
		case Input::Keys::Y:
			return 'Y';
			break;
		case Input::Keys::Z:
			return 'Z';
			break;
		case Input::Keys::NUMPAD0:
			return '0';
			break;
		case Input::Keys::NUMPAD1:
			return '1';
			break;
		case Input::Keys::NUMPAD2:
			return '2';
			break;
		case Input::Keys::NUMPAD3:
			return '3';
			break;
		case Input::Keys::NUMPAD4:
			return '4';
			break;
		case Input::Keys::NUMPAD5:
			return '5';
			break;
		case Input::Keys::NUMPAD6:
			return '6';
			break;
		case Input::Keys::NUMPAD7:
			return '7';
			break;
		case Input::Keys::NUMPAD8:
			return '8';
			break;
		case Input::Keys::NUMPAD9:
			return '9';
			break;
		default:
			return NULL;
			break;
	}
}