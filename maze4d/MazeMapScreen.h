#pragma once

#include <UserInterfaceClasses.h>
#include <MazeMap.h>

class MazeMapScreen : public UserInterfaceItem
{
public:
	MazeMapScreen(Game* game, const Maze& topology, glm::ivec4 currentRoom);

	virtual void Render(uint8_t* buffer);
	virtual UI_ACTION_CODE OnKeyInput(unsigned int keyCode);

private:
	glm::vec2 ToScreen(glm::vec2 mapPosition) const;
	void RenderMapLine(glm::vec2 start, glm::vec2 end, float width, glm::u8vec3 color);
	void RenderMapRectangle(glm::vec2 minPosition, glm::vec2 maxPosition, glm::u8vec4 color);
	void RenderSliceBackground(int y, int w, bool isCurrent);
	void RenderNode(glm::vec2 position, int radius, glm::u8vec3 color);

	MazeMap map;
	glm::ivec4 currentRoom;
	float scale = 1.0f;
	glm::vec2 screenOffset = glm::vec2(0.0f);
};
