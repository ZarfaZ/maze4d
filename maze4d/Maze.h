#pragma once

#include <Utils.h>

struct MazeExit
{
	glm::ivec4 room = glm::ivec4(0);
	int direction = NULL_EDGE;
};

class Maze
{
public:
	Maze(const glm::ivec4 size);
	~Maze();

	bool IsWallExist(int edge, glm::ivec4 pos) const;
	bool HasPassage(int edge, glm::ivec4 pos) const { return !IsWallExist(edge, pos); }
	bool HasExit() const { return exit.direction != NULL_EDGE; }
	const MazeExit& GetExit() const { return exit; }

	void Generate();

	const glm::ivec4 size;

private:
	std::vector<int> FindUnvisitedNeighbours(int curX, int curY, int curZ, int curW);

	int GetIndex(const int x, const int y, const int z, const int w) const;

	bool IsRoomIndexValid(int x, int y, int z, int w) const;
	void GenerateExit();

	struct Room
	{
		bool walls[EDGES_COUNT];
		bool visited;
	};
	Room* rooms = nullptr;
	MazeExit exit;
};
