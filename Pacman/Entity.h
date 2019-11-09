#pragma once
#include "S2D/S2D.h"
#include<string>
#include<vector>

using namespace S2D;

class Entity {
private:
	Vector2* _position;
	Rect* _sourceRect;
	float _rotation;
	std::vector<std::vector<Texture2D*>> _pFrame;
	int _frameCount;
	bool _isStatic;

	int _modelHeight;
	std::string _modelName;

	int _runTime;
	int _currentAnimationFrame;
	bool loopAnimation = true;
	std::string currentAnim;

	void GetAnimationFrameSprites(std::string modelName, int modelHeight, int frame, std::vector<Texture2D*> *result);

public:
	Entity(std::string modelName, int modelHeight, int frameCount, Vector2 initialPos, float initialRotation, bool isStaticObject);
	~Entity();

	void DrawEntity(float elapsedTime);
	
	void ChangeAnimation(std::string animName, int frameCount, bool loop);

	Vector2* GetPosition();
	float GetRotation();
	int GetModelHeight();
	std::string GetModelName();
	bool GetIsStatic();
	std::string GetCurrentAnim();

	void SetPosition(Vector2 vec);
	void SetRotation(float rot);
	void SetModelHeight(int height);
	void SetModelName(std::string name);
	void SetIsStatic(bool isStatic);
};