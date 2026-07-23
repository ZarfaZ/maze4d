#pragma once

#include <Utils.h>

class DirectionPalette
{
public:
	static glm::ivec3 GetHSV(int edge)
	{
		static const glm::ivec3 colors[EDGES_COUNT] = {
			glm::ivec3(351, 86, 80), // NEG_X - red
			glm::ivec3( 32, 86, 80), // POS_X - orange
			glm::ivec3( 62, 86, 80), // NEG_Y - yellow
			glm::ivec3(128, 86, 80), // POS_Y - green
			glm::ivec3(180, 86, 80), // NEG_Z - cyan
			glm::ivec3(245, 86, 80), // POS_Z - blue
			glm::ivec3(282, 86, 80), // NEG_W - purple
			glm::ivec3(  0,  0, 60)  // POS_W - gray
		};

		assert(edge >= 0 && edge < EDGES_COUNT);
		return colors[edge];
	}

	static glm::u8vec3 GetRGB(int edge)
	{
		glm::ivec3 hsv = GetHSV(edge);
		glm::u8vec3 rgb;
		HSVtoRGB(hsv.x, hsv.y, hsv.z, rgb.x, rgb.y, rgb.z);
		return rgb;
	}
};
