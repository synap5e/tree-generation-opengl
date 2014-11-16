#pragma once

#include <stdio.h>
#include <png.h>
#include "glHeaders.hpp"

typedef struct TextureInfo {

	const char *filename;
	unsigned char* textureData;
	int format;
	int width;
	int height;
	bool hasAlpha;

} TextureInfo;

GLuint load_PNG(const char*, TextureInfo*);
