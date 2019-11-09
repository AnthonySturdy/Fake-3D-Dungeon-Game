#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr

//I've disabled this to allow user to chose between pacman and dungeon crawler when the program launches.

//#ifdef WIN32 
//	#ifndef _DEBUG
//		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//	#endif
//#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

#define MUNCHIECOUNT 20

struct sPlayer {
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	int score;
	float speedMultiplier;
};

struct sEnemy {
	Rect* rect;
	Texture2D* texture;
	Vector2* position;
	int inverted;
};

struct sMenu {
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
};


// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	int _frameCount;
	float _runTime;

	// Data to represent Pacman
	sPlayer* _pacman;
	const float _speed;
	const float _animateSpeed;

	// Data to represent Munchie
	sEnemy* _munchies[MUNCHIECOUNT];
	const float _munchieAnimateSpeed;

	// Data to represent cherry
	sEnemy* _cherry;
	const float _cherryAnimateSpeed;

	// Score String
	Vector2* _stringPosition;
	Vector2* _scorePosition;

	// Pause Menu Variables
	sMenu* _pauseMenu;
	bool _paused;
	bool _pauseMenuPressed;

	//Start Menu Variables
	sMenu* _startMenu;
	bool _startMenuOpen;

	//Sounds
	SoundEffect* _pop;

	//Prototypes
	void Input(int elapsedTime, Input::KeyboardState *state, Input::MouseState *mouseState);
	void CheckPaused(Input::KeyboardState *state, Input::Keys pauseKey);
	void WrapViewPort();
	void UpdateAnimations(int elapsedTime);
	void CollisionDetection();

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};