#pragma once
#include"S2D/S2D.h"
#include<string>

using namespace S2D;

class TextRenderer {
private:

public:

	static void RenderText(Vector2* pos, std::string str, Color col, int scale);
	static int GetSpritesheetPosition(char c);

	const static int fontCharacterSize;
	const static std::string fontPath;

};