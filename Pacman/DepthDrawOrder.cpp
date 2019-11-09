#include"DepthDrawOrder.h"
#include<algorithm>

void DepthDrawOrder::Draw(float yPos, Texture2D* texture, Vector2 position, Rect* sourceRectangle, Vector2 origin, float scale, float rotation, Color color, SpriteEffect spriteEffect) {
	//Add sprite information to draw buffer
	drawParameters.push_back(DrawParameters(yPos, texture, position, sourceRectangle, origin, scale, rotation, color, spriteEffect));
}

void DepthDrawOrder::RenderAll() {
	//Sort the vector based on Y position - I had to look online how to do this as I was unsure how to sort multiple vectors based off one vector, this was the simplest technique I could find.
	std::sort(drawParameters.begin(), drawParameters.end(), [](const auto& a, const auto& b) {return a.yPositions > b.yPositions; });

	for (int i = 0; i < drawParameters.size(); i++) {
		SpriteBatch::Draw(drawParameters.at(i).textures, &drawParameters.at(i).positions, drawParameters.at(i).sourceRects, &drawParameters.at(i).origins, drawParameters.at(i).scales, drawParameters.at(i).rotations, &drawParameters.at(i).colours, drawParameters.at(i).spriteEffects);
	}

	drawParameters.clear();
}

int DepthDrawOrder::GetSpriteCount() {
	return drawParameters.size();
}
