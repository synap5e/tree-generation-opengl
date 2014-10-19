//---------------------------------------------------------------------------
//
// Copyright (c) 2012 Taehyun Rhee, Daniel Atkins
//
// This software is provided 'as-is' for assignment of COMP308
// in ECS, Victoria University of Wellington,
// without any express or implied warranty.
// In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------


#pragma once

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jerror.h>
#include <png.h>

//Magic defines for libPNG because they removed them from the library
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#define png_voidp_NULL NULL

/*
 *  The TextureInfo struct. This is where we keep all the texture info when we load a file.
 *  The actual image is stored as an array of unsigned characters (aka unsigned bytes).
 *  It's three bytes per pixel -- one for each color channel, so this array will have a length
 *  of width*height*3. Note that it is ONE dimensional!
 */
typedef struct texInfo {

	const char *filename;
	unsigned char* textureData;
	int format;
	int width;
	int height;
	bool hasAlpha;

} TextureInfo;

int loadTextureFromJPEG(const char*,TextureInfo*);

int loadTextureFromPNG(const char*, TextureInfo*);
