#include <MazeMap.h>

#include <limits>

MazeMap::MazeMap(const Maze& topology) : size(topology.size)
{
	const int xzSpan = glm::max(size.x, size.z);
	yLayerStep = float(xzSpan + 1);
	wLayerStep = float(xzSpan + 1);

	nodes.reserve(size.x * size.y * size.z * size.w);
	for (int x = 0; x < size.x; x++)
		for (int y = 0; y < size.y; y++)
			for (int z = 0; z < size.z; z++)
				for (int w = 0; w < size.w; w++)
				{
					glm::ivec4 room(x, y, z, w);
					nodes.push_back(Node{ room, CalculatePosition(room) });
				}

	static const int positiveDirections[4] = { POS_X, POS_Y, POS_Z, POS_W };
	for (size_t i = 0; i < nodes.size(); i++)
		for (int d = 0; d < 4; d++)
		{
			const int direction = positiveDirections[d];
			if (!topology.IsWallExist(direction, nodes[i].room))
			{
				glm::ivec4 neighbour = GetNeighbour(nodes[i].room, direction);
				edges.push_back(Edge{ i, GetIndex(neighbour), direction });
			}
		}

	if (topology.HasExit())
	{
		hasExit = true;
		exitRoomIndex = GetIndex(topology.GetExit().room);
		exitDirection = topology.GetExit().direction;
		glm::vec2 direction = glm::normalize(GetDirectionVector(exitDirection));
		exitPosition = nodes[exitRoomIndex].position + direction * gridStep * 0.8f;
	}

	CalculateBounds();
}

size_t MazeMap::GetIndex(glm::ivec4 room) const
{
	return size_t(room.x * size.y * size.z * size.w +
		room.y * size.z * size.w +
		room.z * size.w +
		room.w);
}

size_t MazeMap::GetNodeIndex(glm::ivec4 room) const
{
	assert(room.x >= 0 && room.x < size.x);
	assert(room.y >= 0 && room.y < size.y);
	assert(room.z >= 0 && room.z < size.z);
	assert(room.w >= 0 && room.w < size.w);
	return GetIndex(room);
}

glm::vec2 MazeMap::CalculatePosition(glm::ivec4 room) const
{
	glm::vec2 position(0.0f);
	position += GetDirectionVector(POS_X) * float(room.x) * gridStep;
	position += GetDirectionVector(POS_Z) * float(room.z) * gridStep;
	position += GetDirectionVector(POS_Y) * float(room.y) * yLayerStep;
	position += GetDirectionVector(POS_W) * float(room.w) * wLayerStep;
	return position;
}

glm::ivec4 MazeMap::GetNeighbour(glm::ivec4 room, int direction) const
{
	switch (direction)
	{
	case NEG_X: room.x--; break;
	case POS_X: room.x++; break;
	case NEG_Y: room.y--; break;
	case POS_Y: room.y++; break;
	case NEG_Z: room.z--; break;
	case POS_Z: room.z++; break;
	case NEG_W: room.w--; break;
	case POS_W: room.w++; break;
	default: assert(false); break;
	}
	return room;
}

glm::vec2 MazeMap::GetDirectionVector(int direction)
{
	// Keep layer offsets incommensurate with the square X/Z grid. At 45 degrees
	// Y/W edges repeatedly pass through vertices of neighbouring layers.
	static const float layerAngle = 37.0f;
	static const float layerX = cosd(layerAngle);
	static const float layerY = sind(layerAngle);

	switch (direction)
	{
	case NEG_X: return glm::vec2( 0.0f, -1.0f);
	case POS_X: return glm::vec2( 0.0f,  1.0f);
	case NEG_Y: return glm::vec2(-layerX, -layerY);
	case POS_Y: return glm::vec2( layerX,  layerY);
	case NEG_Z: return glm::vec2(-1.0f,  0.0f);
	case POS_Z: return glm::vec2( 1.0f,  0.0f);
	case NEG_W: return glm::vec2( layerX, -layerY);
	case POS_W: return glm::vec2(-layerX,  layerY);
	default: assert(false); return glm::vec2(0.0f);
	}
}

void MazeMap::CalculateBounds()
{
	const float maxFloat = std::numeric_limits<float>::max();
	minBounds = glm::vec2(maxFloat);
	maxBounds = glm::vec2(-maxFloat);

	for (size_t i = 0; i < nodes.size(); i++)
	{
		minBounds = glm::min(minBounds, nodes[i].position);
		maxBounds = glm::max(maxBounds, nodes[i].position);
	}

	if (hasExit)
	{
		minBounds = glm::min(minBounds, exitPosition);
		maxBounds = glm::max(maxBounds, exitPosition);
	}
}
