#include "TextRenderer.h"
#include<vector>

const int TextRenderer::fontCharacterSize = 9;
const std::string TextRenderer::fontPath = "DungeonCrawler_Assets/Fonts/Hoshi.png";

void TextRenderer::RenderText(Vector2* pos, std::string str, Color col, int scale) {
	Texture2D* fontTexture = new Texture2D();
	fontTexture->Load(TextRenderer::fontPath.c_str(), false);

	Rect* sourceRect = new Rect();

	int y_offset = 0;
	int x_pos = 0;

	for (int i = 0; i < strlen(str.c_str()); i++) {
		sourceRect->X = TextRenderer::GetSpritesheetPosition(str[i]) * TextRenderer::fontCharacterSize;
		sourceRect->Y = 0;
		sourceRect->Width = TextRenderer::fontCharacterSize;
		sourceRect->Height = TextRenderer::fontCharacterSize;

		if (str.at(i) == '\n') {
			y_offset++;
			x_pos = -1;
		} else {
			SpriteBatch::Draw(fontTexture, &Vector2(pos->X + (x_pos * TextRenderer::fontCharacterSize * scale), pos->Y + (y_offset * TextRenderer::fontCharacterSize)), sourceRect, &Vector2(0, 0), scale, 0, &col, SpriteEffect::NONE);
		}

		x_pos++;
	}

	delete fontTexture;
	fontTexture = nullptr;
	delete sourceRect;
	sourceRect = nullptr;
}

int TextRenderer::GetSpritesheetPosition(char c) {
	switch (c) {
	case '!':
		return 0;
		break;
	case '"':
		return 1;
		break;
	case '&':
		return 5;
		break;
	case '(':
		return 7;
		break;
	case ')':
		return 8;
		break;
	case ',':
		return 11;
		break;
	case '-':
		return 12;
		break;
	case '.':
		return 13;
		break;
	case '0':
		return 15;
		break;
	case '1':
		return 16;
		break;
	case '2':
		return 17;
		break;
	case '3':
		return 18;
		break;
	case '4':
		return 19;
		break;
	case '5':
		return 20;
		break;
	case '6':
		return 21;
		break;
	case '7':
		return 22;
		break;
	case '8':
		return 23;
		break;
	case '9':
		return 24;
		break;
	case ':':
		return 25;
		break;
	case '<':
		return 27;
		break;
	case '>':
		return 29;
		break;
	case 'A':
		return 32;
		break;
	case 'B':
		return 33;
		break;
	case 'C':
		return 34;
		break;
	case 'D':
		return 35;
		break;
	case 'E':
		return 36;
		break;
	case 'F':
		return 37;
		break;
	case 'G':
		return 38;
		break;
	case 'H':
		return 39;
		break;
	case 'I':
		return 40;
		break;
	case 'J':
		return 41;
		break;
	case 'K':
		return 42;
		break;
	case 'L':
		return 43;
		break;
	case 'M':
		return 44;
		break;
	case 'N':
		return 45;
		break;
	case 'O':
		return 46;
		break;
	case 'P':
		return 47;
		break;
	case 'Q':
		return 48;
		break;
	case 'R':
		return 49;
		break;
	case 'S':
		return 50;
		break;
	case 'T':
		return 51;
		break;
	case 'U':
		return 52;
		break;
	case 'V':
		return 53;
		break;
	case 'W':
		return 54;
		break;
	case 'X':
		return 55;
		break;
	case 'Y':
		return 56;
		break;
	case 'Z':
		return 57;
		break;
	case '\'':
		return 63;
		break;
	case 'a':
		return 64;
		break;
	case 'b':
		return 65;
		break;
	case 'c':
		return 66;
		break;
	case 'd':
		return 67;
		break;
	case 'e':
		return 68;
		break;
	case 'f':
		return 69;
		break;
	case 'g':
		return 70;
		break;
	case 'h':
		return 71;
		break;
	case 'i':
		return 72;
		break;
	case 'j':
		return 73;
		break;
	case 'k':
		return 74;
		break;
	case 'l':
		return 75;
		break;
	case 'm':
		return 76;
		break;
	case 'n':
		return 77;
		break;
	case 'o':
		return 78;
		break;
	case 'p':
		return 79;
		break;
	case 'q':
		return 80;
		break;
	case 'r':
		return 81;
		break;
	case 's':
		return 82;
		break;
	case 't':
		return 83;
		break;
	case 'u':
		return 84;
		break;
	case 'v':
		return 85;
		break;
	case 'w':
		return 86;
		break;
	case 'x':
		return 87;
		break;
	case 'y':
		return 88;
		break;
	case 'z':
		return 89;
		break;
	case ' ':
		return 95;
		break;
	}
}