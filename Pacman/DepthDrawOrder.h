#pragma once
#include "S2D/S2D.h"
#include<vector>

using namespace S2D;

class DepthDrawOrder {
private:
	struct DrawParameters {
		float yPositions;
		Texture2D* textures;
		Vector2 positions;
		Rect* sourceRects;
		Vector2 origins;
		float scales;
		float rotations;
		Color colours;
		SpriteEffect spriteEffects;

		DrawParameters(float yPos, Texture2D* texture, Vector2 position, Rect* sourceRectangle, Vector2 origin, float scale, float rotation, Color color, SpriteEffect spriteEffect) {
			yPositions = yPos;
			textures = texture;
			positions = position;
			sourceRects = sourceRectangle;
			origins = origin;
			scales = scale;
			rotations = rotation;
			colours = color;
			spriteEffects = spriteEffect;
		}
	};

	std::vector<DrawParameters> drawParameters;

public:

	void Draw(float yPos, Texture2D* texture, Vector2 position, Rect* sourceRectangle, Vector2 origin, float scale, float rotation, Color color, SpriteEffect spriteEffect);

	void RenderAll();

	int GetSpriteCount();

};