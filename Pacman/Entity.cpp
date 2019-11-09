#include"Entity.h"
#include"DungeonCrawler.h"

Entity::Entity(std::string modelName, int modelHeight, int frameCount, Vector2 initialPos, float initialRotation, bool isStaticObject) {
	_position = new Vector2(initialPos.X, initialPos.Y);
	_rotation = initialRotation;
	_frameCount = frameCount;

	for (int i = 0; i < frameCount; i++) {
		_pFrame.push_back(std::vector<Texture2D*>());
		GetAnimationFrameSprites(modelName, modelHeight, i, &_pFrame.at(i));
		_sourceRect = new Rect(0.0f, 0.0f, _pFrame.at(0).at(0)->GetWidth(), _pFrame.at(0).at(0)->GetHeight());
	}

	SetIsStatic(isStaticObject);
	SetModelHeight(modelHeight);
	SetModelName(modelName);
}

Entity::~Entity() {
	delete _position;
	delete _sourceRect;
	for (int curFrame = 0; curFrame < _frameCount; curFrame++) {
		for (int i = 0; i < _pFrame.at(curFrame).size(); i++) {
			delete  _pFrame.at(curFrame).at(i);
		}

		_pFrame.at(curFrame).clear();
	}
	_pFrame.clear();
}

//Draws this Entity
void Entity::DrawEntity(float elapsedTime) {
	_runTime += elapsedTime;

	//If animation is only one loop, set back to default after
	if (!loopAnimation && _currentAnimationFrame == _frameCount) {
		ChangeAnimation("", 1, true);
	}

	for (int i = 0; i < GetModelHeight(); i++) {
		Vector2* vec;
		Vector2* origin;
		if (!GetIsStatic()) {	//Player/Enemies/etc.
			//Calculate Sprite Origin and Draw Position
			origin = new Vector2(_pFrame.at(_currentAnimationFrame % _frameCount).at(i)->GetWidth() / 2, GetModelHeight() - (_pFrame.at(_currentAnimationFrame % _frameCount).at(i)->GetHeight() / 2));
			vec = new Vector2(Graphics::GetViewportWidth()/2, Graphics::GetViewportHeight() / 2 - (i * 2));
		}
		else {
			//Calculate Sprite Origin and Draw Position
			Vector2 MiddleScreen(Graphics::GetViewportWidth() / 2, Graphics::GetViewportHeight() / 2 - (i * 2));
			origin = new Vector2(DungeonCrawler::_playerObject->GetPosition()->X - GetPosition()->X, DungeonCrawler::_playerObject->GetPosition()->Y - GetPosition()->Y);
			Vector2 offset(DungeonCrawler::_playerObject->GetPosition()->X - GetPosition()->X, DungeonCrawler::_playerObject->GetPosition()->Y - GetPosition()->Y);		//Offset from Tile to Player Pos
			vec = new Vector2(origin->X - offset.X + MiddleScreen.X, origin->Y - offset.Y + MiddleScreen.Y);
		}

		//Draw Sprite
		DungeonCrawler::drawOrder->Draw(vec->Y, _pFrame.at(_currentAnimationFrame % _frameCount).at(i), *vec, _sourceRect, *origin, 3.0f, _rotation, *Color::White, SpriteEffect::FLIPVERTICAL);

		delete vec;
		delete origin;
		vec = nullptr;
		origin = nullptr;
	}

	const int animationRate = 160;

	if (_runTime > animationRate) {
		_currentAnimationFrame++;
		_runTime = 0;
	}
}

void Entity::ChangeAnimation(std::string animName, int frameCount, bool loop) {
	//First remove all current texture data
	for (int curFrame = 0; curFrame < _frameCount; curFrame++) {
		for (int i = 0; i < _pFrame.at(curFrame).size(); i++) {
			delete  _pFrame.at(curFrame).at(i);
		}

		_pFrame.at(curFrame).clear();
	}
	_pFrame.clear();

	//Load new textures for new animation
	_frameCount = frameCount;
	for (int i = 0; i < frameCount; i++) {
		_pFrame.push_back(std::vector<Texture2D*>());
		GetAnimationFrameSprites(GetModelName() + animName, GetModelHeight(), i, &_pFrame.at(i));
	}

	_currentAnimationFrame = 0;
	currentAnim = animName;
	loopAnimation = loop;
}

//Gathers all sprites for one frame from folder
void Entity::GetAnimationFrameSprites(std::string modelName, int modelHeight, int frame, std::vector<Texture2D*> *result) {
	for (int i = 0; i < modelHeight; i++) {
		std::string dir;
		dir = "DungeonCrawler_Assets/Models/" + modelName + "/" + "frame" + std::to_string(frame) + "/" + modelName + std::to_string(i) + ".png";
		(*result).push_back(new Texture2D());
		(*result)[i]->Load(dir.c_str(), false);
	}
}


Vector2* Entity::GetPosition() {
	return _position;
}

float Entity::GetRotation() {
	return _rotation;
}

int Entity::GetModelHeight() {
	return _modelHeight;
}
std::string Entity::GetModelName() {
	return _modelName;
}

bool Entity::GetIsStatic() {
	return _isStatic;
}

std::string Entity::GetCurrentAnim() {
	return currentAnim;
}

void Entity::SetPosition(Vector2 vec) {
	*_position = vec;
}
void Entity::SetRotation(float rot) {
	_rotation = rot;
}

void Entity::SetModelHeight(int height) {
	_modelHeight = height;
}

void Entity::SetModelName(std::string name) {
	_modelName = name;
}

void Entity::SetIsStatic(bool isStatic) {
	_isStatic = isStatic;
}

