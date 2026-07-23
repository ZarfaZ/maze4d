#pragma once


#include <Field.h>
#include <Maze.h>
#include <Config.h>

class Field;

class MazeField
{
public:
	//Allocates with NEW function, needs to be deleted manually once done
	Field* GenerateField(const Maze& maze, const glm::ivec4 size, const int lightDist, const int roomSize, Config* cfg);

	int roomSize;

	Field* field = nullptr;
private:

	void CreateExit(const Maze* maze);

	void GenerateWalls(const Maze* maze);

	void GenerateLight(const Maze* maze);

	
//	static const glm::ivec4 winMapSize;
	
};
