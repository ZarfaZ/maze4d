#include <Cube.h>
#include <DirectionPalette.h>

Cube::TextureSet_t Cube::textureSet;
Texture Cube::textureSelection;
Texture Cube::textureLight;

void Cube::Init(Shader* shader)
{
	for (int edge = 0; edge < EDGES_COUNT; edge++)
		InitBlockTexture(textureSet[edge], DirectionPalette::GetHSV(edge), shader);
	InitSelectionTexture(textureSelection, shader);
	InitLightTexture(textureLight, shader);

	// block textures
	GLint loc = glGetUniformLocation(shader->ID, "edge3dCube");
	GLint* edgeTextureUnits = new GLint[8];
	for (int edge = 0; edge < EDGES_COUNT; edge++)
	{
		edgeTextureUnits[edge] = 50 + edge;
	}
	glUniform1iv(loc, EDGES_COUNT, edgeTextureUnits);
	delete[] edgeTextureUnits;

	// selection texture
	loc = glGetUniformLocation(shader->ID, "selection3dCube");
	glUniform1i(loc, 50 + EDGES_COUNT);

	// light texture
	loc = glGetUniformLocation(shader->ID, "light3dCube");
	glUniform1i(loc, 50 + EDGES_COUNT + 1);
}

void Cube::BindTextures()
{
	// block textures
	for (int edge = 0; edge < EDGES_COUNT; edge++)
	{
		glActiveTexture(GL_TEXTURE0 + 50 + edge);
		glBindTexture(GL_TEXTURE_3D, textureSet[edge].textureId);
	}

	// selection texture
	glActiveTexture(GL_TEXTURE0 + 50 + EDGES_COUNT);
	glBindTexture(GL_TEXTURE_3D, textureSelection.textureId);

	// light texture
	glActiveTexture(GL_TEXTURE0 + 50 + EDGES_COUNT + 1);
	glBindTexture(GL_TEXTURE_3D, textureLight.textureId);
}

void Cube::InitBlockTexture(Texture& tex, glm::ivec3 colorHSV, Shader* shader)
{
	glm::u8vec4 *texBuffer = new glm::u8vec4[Texture::GetSize()];

	Random* rnd = Random::GetInstance();
	for (int x = 0; x < Texture::TEX_SIZE; x++)
	{
		for (int y = 0; y < Texture::TEX_SIZE; y++)
		{
			for (int z = 0; z < Texture::TEX_SIZE; z++)
			{
				glm::u8vec4& pixel = Texture::GetPixel(x, y, z, texBuffer);

				int initValue = rnd->GetInt(glm::max(0, colorHSV[2] - 20), glm::min(100, colorHSV[2] + 20));

				int minBorder = Texture::BORDER_SIZE;
				int maxBorder = (int)(Texture::TEX_SIZE - Texture::BORDER_SIZE);
				if (x < minBorder || x >= maxBorder || y < minBorder || y >= maxBorder || z < minBorder || z >= maxBorder)
					initValue = 20;

				HSVtoRGB(colorHSV[0], colorHSV[1], initValue, pixel.x, pixel.y, pixel.z);
				pixel.w = 255;
			}
		}
	}

	Texture::GenerateGlTexture(tex.textureId, texBuffer);

	delete[] texBuffer;
}

void Cube::InitSelectionTexture(Texture& tex, Shader* shader)
{
	glm::u8vec4 *texBuffer = new glm::u8vec4[Texture::GetSize()];

	for (int x = 0; x < Texture::TEX_SIZE; x++)
	{
		for (int y = 0; y < Texture::TEX_SIZE; y++)
		{
			for (int z = 0; z < Texture::TEX_SIZE; z++)
			{
				glm::u8vec4& pixel = Texture::GetPixel(x, y, z, texBuffer);

				int minBorder = 1;
				int maxBorder = Texture::TEX_SIZE - 1;
				if (x < minBorder || x >= maxBorder || y < minBorder || y >= maxBorder || z < minBorder || z >= maxBorder)
					pixel = glm::u8vec4(255, 255, 255, 255);
				else
					pixel = glm::u8vec4(0, 0, 0, 0);
			}
		}
	}

	Texture::GenerateGlTexture(tex.textureId, texBuffer);

	delete[] texBuffer;
}

void Cube::InitLightTexture(Texture& tex, Shader* shader)
{
	glm::u8vec4 *texBuffer = new glm::u8vec4[Texture::GetSize()];

	for (int x = 0; x < Texture::TEX_SIZE; x++)
	{
		for (int y = 0; y < Texture::TEX_SIZE; y++)
		{
			for (int z = 0; z < Texture::TEX_SIZE; z++)
			{
				glm::u8vec4& pixel = Texture::GetPixel(x, y, z, texBuffer);

				int minBorder = Texture::BORDER_SIZE;
				int maxBorder = Texture::TEX_SIZE - Texture::BORDER_SIZE;
				if (x < minBorder || x >= maxBorder || y < minBorder || y >= maxBorder || z < minBorder || z >= maxBorder)
					pixel = glm::u8vec4(255, 233, 68, 255);
				else
					pixel = glm::u8vec4(255, 252, 229, 235);
			}
		}
	}

	Texture::GenerateGlTexture(tex.textureId, texBuffer);

	delete[] texBuffer;
}
