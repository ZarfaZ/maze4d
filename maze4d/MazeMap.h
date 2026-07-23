#pragma once

#include <Maze.h>

class MazeMap
{
public:
	struct Node
	{
		glm::ivec4 room;
		glm::vec2 position;
	};

	struct Edge
	{
		size_t from;
		size_t to;
		int direction;
	};

	explicit MazeMap(const Maze& topology);

	const std::vector<Node>& GetNodes() const { return nodes; }
	const std::vector<Edge>& GetEdges() const { return edges; }
	const glm::ivec4& GetSize() const { return size; }
	const glm::vec2& GetMinBounds() const { return minBounds; }
	const glm::vec2& GetMaxBounds() const { return maxBounds; }
	bool HasExit() const { return hasExit; }
	glm::vec2 GetExitPosition() const { return exitPosition; }
	glm::vec2 GetExitRoomPosition() const { return nodes[exitRoomIndex].position; }
	int GetExitDirection() const { return exitDirection; }
	size_t GetNodeIndex(glm::ivec4 room) const;

	static glm::vec2 GetDirectionVector(int direction);
	static int GetOppositeDirection(int direction) { return direction ^ 1; }

private:
	size_t GetIndex(glm::ivec4 room) const;
	glm::vec2 CalculatePosition(glm::ivec4 room) const;
	glm::ivec4 GetNeighbour(glm::ivec4 room, int direction) const;
	void CalculateBounds();

	glm::ivec4 size;
	float gridStep = 1.0f;
	float yLayerStep = 1.0f;
	float wLayerStep = 1.0f;
	std::vector<Node> nodes;
	std::vector<Edge> edges;
	glm::vec2 minBounds = glm::vec2(0.0f);
	glm::vec2 maxBounds = glm::vec2(0.0f);
	bool hasExit = false;
	size_t exitRoomIndex = 0;
	glm::vec2 exitPosition = glm::vec2(0.0f);
	int exitDirection = NULL_EDGE;
};
