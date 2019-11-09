#include "Pacman.h"
#include <time.h>
#include <sstream>


Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _speed(0.2f), _animateSpeed(0.5f), _munchieAnimateSpeed(1.0f), _cherryAnimateSpeed(0.2f)
{
	srand(time(NULL));
	_frameCount = 0;
	_runTime = 0.0f;
	_paused = false;

	_pacman = new sPlayer();
	for (int i = 0; i < MUNCHIECOUNT; i++) {
		_munchies[i] = new sEnemy();
	}

	_cherry = new sEnemy();
	_pauseMenu = new sMenu();
	_startMenu = new sMenu();

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	for (int i = 0; i < MUNCHIECOUNT; i++) {
		delete _munchies[i]->texture;
		delete _munchies[i]->rect;
		delete _munchies[i]->position;
	}
	delete _cherry->texture;
	delete _cherry->rect;
	delete _cherry->position;
	delete _pauseMenu->_menuBackground;
	delete _pauseMenu->_menuRectangle;
	delete _pauseMenu->_menuStringPosition;
	delete _startMenu->_menuBackground;
	delete _startMenu->_menuRectangle;
	delete _startMenu->_menuStringPosition;

	delete _pacman;
	for (int i = 0; i < MUNCHIECOUNT; i++) {
		delete _munchies[i];
	}
	delete _cherry;
	delete _pauseMenu;
	delete _startMenu;
	delete _pop;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacman->score = 0;
	_pacman->speedMultiplier = 1.0f;

	// Load Munchie
	for (int i = 0; i < MUNCHIECOUNT; i++) {
		_munchies[i]->texture = new Texture2D();
		_munchies[i]->texture->Load("Textures/Munchie.png", true);
		_munchies[i]->rect = new Rect(0.0f, 0.0f, 12, 12);
		_munchies[i]->position = new Vector2(100.0f, 100.0f);
	}

	// Load Munchie
	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/Cherry.png", true);
	_cherry->rect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry->position = new Vector2(200.0f, 100.0f);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	_scorePosition = new Vector2(10.0f, 60.0f);

	// Load Menu Parameters
	_pauseMenu->_menuBackground = new Texture2D();
	_pauseMenu->_menuBackground->Load("Textures/Transparency.png", false);
	_pauseMenu->_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_pauseMenu->_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_pauseMenuPressed = false;

	// Load Start Menu Parameters
	_startMenu->_menuBackground = new Texture2D();
	_startMenu->_menuBackground->Load("Textures/startmenu.png", false);
	_startMenu->_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_startMenu->_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f - 80, Graphics::GetViewportHeight() / 2.0f + 250);
	_startMenuOpen = true;

	// Load Sound Effects
	_pop = new SoundEffect();
	_pop->Load("Sounds/pop.wav");
}

void Pacman::Update(int elapsedTime)
{
	CheckPaused(Input::Keyboard::GetState(), Input::Keys::P);

	if (!_paused || _startMenuOpen) {
		Input(elapsedTime, Input::Keyboard::GetState(), Input::Mouse::GetState());
		
		UpdateAnimations(elapsedTime);

		WrapViewPort();

		CollisionDetection();

		_frameCount++;
		_runTime += (elapsedTime / 1000.0f);
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;

	std::stringstream scoreStream;
	scoreStream << "Score: " << _pacman->score;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman

	for (int i = 0; i < MUNCHIECOUNT; i++) {
		SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->rect);
	}
	SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->rect);

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawString(scoreStream.str().c_str(), _scorePosition, Color::Green);

	// Draw Menu
	if (_paused) {
		std::stringstream menuStream;
		menuStream << "Paused!";

		SpriteBatch::Draw(_pauseMenu->_menuBackground, _pauseMenu->_menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _pauseMenu->_menuStringPosition, Color::Red);
	}

	if (_startMenuOpen) {
		std::stringstream startMenuText;
		startMenuText << "Press Enter to Start Game!";

		SpriteBatch::Draw(_startMenu->_menuBackground, _startMenu->_menuRectangle, nullptr);
		SpriteBatch::DrawString(startMenuText.str().c_str(), _startMenu->_menuStringPosition, Color::Yellow);
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}


void Pacman::Input(int elapsedTime, Input::KeyboardState *keyboardState, Input::MouseState *mouseState) {
	_pacman->speedMultiplier = keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT) ? 2.0f : 1.0f;
	float pacSpeed = _speed * elapsedTime * _pacman->speedMultiplier;

	// Checks for movement input
	if (keyboardState->IsKeyDown(Input::Keys::D)) {
		_pacman->position->X += pacSpeed; //Moves Pacman across X axis
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * 0;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::A)) {
		_pacman->position->X -= pacSpeed; //Moves Pacman across X axis
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * 2;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::S)) {
		_pacman->position->Y += pacSpeed; //Moves Pacman across Y axis
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * 1;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::W)) {
		_pacman->position->Y -= pacSpeed; //Moves Pacman across Y axis
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * 3;
	}

	if (mouseState->LeftButton == Input::ButtonState::PRESSED) {
		for (int i = 0; i < MUNCHIECOUNT; i++) {
			_munchies[i]->position->X = mouseState->X;
			_munchies[i]->position->Y = mouseState->Y;
		}
	}
}

void Pacman::CheckPaused(Input::KeyboardState *state, Input::Keys pauseKey) {
	if (state->IsKeyDown(pauseKey) && !_pauseMenuPressed) {
		_paused = !_paused;
		_pauseMenuPressed = true;
	}
	if (state->IsKeyUp(pauseKey)) {
		_pauseMenuPressed = false;
	}

	if (state->IsKeyDown(Input::Keys::RETURN)) {
		_startMenuOpen = false;
	}
}

void Pacman::WrapViewPort() {
	//Wrap Pacman around screen
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth())
		_pacman->position->X = 0;
	else if (_pacman->position->X + _pacman->sourceRect->Width < 0)
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;
	else if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight())
		_pacman->position->Y = 0;
	else if (_pacman->position->Y + _pacman->sourceRect->Height < 0)
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
}

void Pacman::UpdateAnimations(int elapsedTime) {
	//Animate Pacman
	if (std::fmod(_runTime, _animateSpeed) < _animateSpeed / 2)
	{
		_pacman->sourceRect->X = _pacman->sourceRect->Width * 1;
	}
	else
	{
		_pacman->sourceRect->X = _pacman->sourceRect->Width * 0;
	}

	for (int i = 0; i < MUNCHIECOUNT; i++) {
		//Animate Munchie
		if (std::fmod(_runTime, _munchieAnimateSpeed) < _munchieAnimateSpeed / 2)
		{
			// Draws Red Munchie
			_munchies[i]->rect->X = _munchies[i]->rect->Width * 1;
		} else
		{
			// Draw Blue Munchie
			_munchies[i]->rect->X = _munchies[i]->rect->Width * 0;
		}
	}

	//Animate Cherry
	if (std::fmod(_runTime, _cherryAnimateSpeed) < _cherryAnimateSpeed / 2)
	{
		// Draws Red Munchie
		_cherry->rect->X = _cherry->rect->Width * 1;
	}
	else
	{
		// Draw Blue Munchie
		_cherry->rect->X = _cherry->rect->Width * 0;
	}
}

void Pacman::CollisionDetection() {
	for (int i = 0; i < MUNCHIECOUNT; i++) {
		//Munchie Collision Detection
		if (_pacman->position->X < _munchies[i]->position->X + _munchies[i]->rect->Width &&
			_pacman->position->X + _pacman->sourceRect->Width > _munchies[i]->position->X &&
			_pacman->position->Y < _munchies[i]->position->Y + _munchies[i]->rect->Height &&
			_pacman->position->Y + _pacman->sourceRect->Height > _munchies[i]->position->Y) {


			_munchies[i]->position = new Vector2(rand() % Graphics::GetViewportWidth(), std::rand() % Graphics::GetViewportHeight());
			_pacman->score++;
			Audio::Play(_pop);
		}
	}
	

	//Cherry Collision Detection
	if (_pacman->position->X < _cherry->position->X + _cherry->rect->Width &&
		_pacman->position->X + _pacman->sourceRect->Width > _cherry->position->X &&
		_pacman->position->Y < _cherry->position->Y + _cherry->rect->Height &&
		_pacman->position->Y + _pacman->sourceRect->Height > _cherry->position->Y) {


		_cherry->position = new Vector2(rand() % Graphics::GetViewportWidth(), std::rand() % Graphics::GetViewportHeight());
		_pacman->score += 5;
	}
}