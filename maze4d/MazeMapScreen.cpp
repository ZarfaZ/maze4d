#include <MazeMapScreen.h>

#include <DirectionPalette.h>

MazeMapScreen::MazeMapScreen(Game* game, const Maze& topology, glm::ivec4 currentRoom)
	: UserInterfaceItem(game), map(topology), currentRoom(currentRoom)
{
}

void MazeMapScreen::Render(uint8_t* buffer)
{
	const int viewWidth = game->viewWidth;
	const int viewHeight = game->viewHeight;
	const int padding = glm::max(30, glm::min(viewWidth, viewHeight) / 16);
	const glm::vec2 boundsSize = map.GetMaxBounds() - map.GetMinBounds();
	const float mapWidth = glm::max(boundsSize.x, 1.0f);
	const float mapHeight = glm::max(boundsSize.y, 1.0f);
	const float availableWidth = float(glm::max(viewWidth - padding * 2, 1));
	const float availableHeight = float(glm::max(viewHeight - padding * 2, 1));
	scale = glm::min(availableWidth / mapWidth, availableHeight / mapHeight);

	const glm::vec2 renderedSize(boundsSize.x * scale, boundsSize.y * scale);
	screenOffset = glm::vec2(
		(float(viewWidth) - renderedSize.x) * 0.5f,
		(float(viewHeight) - renderedSize.y) * 0.5f);

	RenderRectangle(0, 0, viewWidth, viewHeight, glm::u8vec3(12, 14, 18));

	const std::vector<MazeMap::Node>& nodes = map.GetNodes();
	const std::vector<MazeMap::Edge>& edges = map.GetEdges();
	const float edgeWidth = glm::clamp(scale * 0.07f, 1.5f, 4.0f);

	for (size_t i = 0; i < edges.size(); i++)
	{
		const MazeMap::Edge& edge = edges[i];
		glm::vec2 start = ToScreen(nodes[edge.from].position);
		glm::vec2 end = ToScreen(nodes[edge.to].position);
		glm::vec2 middle = (start + end) * 0.5f;

		RenderMapLine(start, middle, edgeWidth, DirectionPalette::GetRGB(edge.direction));
		RenderMapLine(middle, end, edgeWidth,
			DirectionPalette::GetRGB(MazeMap::GetOppositeDirection(edge.direction)));
	}

	if (map.HasExit())
	{
		glm::vec2 exitRoom = ToScreen(map.GetExitRoomPosition());
		glm::vec2 exitPosition = ToScreen(map.GetExitPosition());
		RenderMapLine(exitRoom, exitPosition, edgeWidth * 1.5f,
			DirectionPalette::GetRGB(map.GetExitDirection()));

		const int exitRadius = glm::max(5, (int)glm::clamp(scale * 0.16f, 5.0f, 9.0f));
		RenderNode(exitPosition, exitRadius, glm::u8vec3(110, 255, 130));
		RenderMapLine(
			exitPosition + glm::vec2(-exitRadius, -exitRadius),
			exitPosition + glm::vec2(exitRadius, exitRadius),
			2.0f, glm::u8vec3(15, 45, 20));
		RenderMapLine(
			exitPosition + glm::vec2(-exitRadius, exitRadius),
			exitPosition + glm::vec2(exitRadius, -exitRadius),
			2.0f, glm::u8vec3(15, 45, 20));
	}

	const size_t currentIndex = map.GetNodeIndex(currentRoom);
	const int nodeRadius = glm::max(2, (int)glm::clamp(scale * 0.10f, 2.0f, 6.0f));
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (i == currentIndex)
			continue;
		RenderNode(ToScreen(nodes[i].position), nodeRadius, glm::u8vec3(225, 225, 225));
	}

	RenderNode(ToScreen(nodes[currentIndex].position), nodeRadius + 3, glm::u8vec3(255, 235, 80));
}

UI_ACTION_CODE MazeMapScreen::OnKeyInput(unsigned int keyCode)
{
	if (keyCode == GLFW_KEY_M)
		return UI_ACTION_CLOSE_ITEM;
	return UI_ACTION_NOTHING;
}

glm::vec2 MazeMapScreen::ToScreen(glm::vec2 mapPosition) const
{
	return screenOffset + (mapPosition - map.GetMinBounds()) * scale;
}

void MazeMapScreen::RenderMapLine(glm::vec2 start, glm::vec2 end, float width, glm::u8vec3 color)
{
	glm::vec2 delta = end - start;
	if (glm::length(delta) <= 0.0001f)
		return;

	glm::vec2 perpendicular = glm::normalize(glm::vec2(-delta.y, delta.x)) * width * 0.5f;
	glm::vec2 firstLeft = start + perpendicular;
	glm::vec2 firstRight = start - perpendicular;
	glm::vec2 secondRight = end - perpendicular;
	glm::vec2 secondLeft = end + perpendicular;

	const float viewWidth = (float)game->viewWidth;
	const float viewHeight = (float)game->viewHeight;
	auto toViewport = [&](glm::vec2 point)
	{
		return glm::vec2(
			2.0f * point.x / viewWidth - 1.0f,
			2.0f * point.y / viewHeight - 1.0f);
	};

	graphics->SetNewQuad(
		toViewport(firstLeft),
		toViewport(firstRight),
		toViewport(secondRight),
		toViewport(secondLeft));
	graphics->Draw(color);
}

void MazeMapScreen::RenderNode(glm::vec2 position, int radius, glm::u8vec3 color)
{
	RenderRectangle(
		(int)glm::round(position.x) - radius,
		(int)glm::round(position.y) - radius,
		radius * 2 + 1,
		radius * 2 + 1,
		color);
}
