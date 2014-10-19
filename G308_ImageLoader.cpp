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

#include "G308_ImageLoader.hpp"
#include "string.h"

/*
 * Magical JPEG loader. You probably shouldn't edit this.
 */
int loadTextureFromJPEG(const char* filename, TextureInfo *info) {

	//Gonna need a file pointer.
	FILE *fd;

	//Init the structs required by libjpeg
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	//This will keep track of where we are in the file
	unsigned char * line;

	//Redirect stderr so things aren't messy
	cinfo.err = jpeg_std_error(&jerr);

	//Init the decompress struct
	jpeg_create_decompress(&cinfo);

	//Try to open the file. Better code would return an error value,
	//but I'm just making it exit instead.
	if (0 == (fd = fopen(filename, "rb"))) {
		printf("Error opening file");
		exit(1);
	}
	//Self evident.
	info->filename = filename;
	//Point libjpeg at the file
	jpeg_stdio_src(&cinfo, fd);
	//Read in the JPEG header
	jpeg_read_header(&cinfo, TRUE);
	//Used to keep track of offset in 1-D pixel array
	unsigned int size = cinfo.image_width;

	//Populate the texture-info struct.
	//If it's not GL_RGB, then you have a really strange JPEG.
	info->height = cinfo.image_height;
	info->width = cinfo.image_width;
	info->format = GL_RGB;
	//If your JPEG somehow has alpha, you're gonna have a bad time.
	info->hasAlpha = false;

	//Assign an array for pixels
	unsigned char* image = (unsigned char*) malloc(sizeof(char) * 3 * cinfo.image_height * cinfo.image_width);

	//Begin magic.
	jpeg_start_decompress(&cinfo);
	while (cinfo.output_scanline < cinfo.output_height) {
		line = image
				+ (3 * size) * cinfo.output_scanline;
		jpeg_read_scanlines(&cinfo, &line, 1);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	//Allocate an array for the pixels. Why two? You'll see.
	info->textureData = (unsigned char*) malloc(sizeof(char) * 3 * cinfo.image_height * cinfo.image_width);

	//Copy the pixels from image to textureData in reverse row order. Because GL.
	for(int y = info->height-1; y >= 0; y--){
		for(int x = 0; x < info->width*3; x++){
				info->textureData[(info->height-y-1)*(info->width*3)+x] = image[y*info->width*3+x];
		}
	}
	free(image);

	return 0;
}

/*
 * Magical PNG loader. You probably shouldn't edit this.
 */
int loadTextureFromPNG(const char* filename , TextureInfo* t){

	//Required structs and file pointers.
	png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    FILE *fp;

    //Again, this should return an error code instead of exiting,
    //but for this assignment, it should work anyway.
    if ((fp = fopen(filename, "rb")) == NULL){
        printf("Error opening %s\n",filename);
        exit(1);
    }

    //Magic, do not touch.
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL) {
    	printf("LibPNG did something terrible!\n");
        fclose(fp);
        exit(1);
    }

    //Allocate the struct we're going to read the data into.
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
    	printf("LibPNG did something terrible!\n");
        fclose(fp);
        png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
        exit(1);
    }

    //Error handling magic. Do not touch.
    if (setjmp(png_jmpbuf(png_ptr))) {
    	printf("LibPNG did something terrible!\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        fclose(fp);
        exit(1);
    }

    //Point libPNG to the right place
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, sig_read);

    //Yet more magic you shouldn't touch. We -probably- have enough memory to read it all in one go.
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, png_voidp_NULL);

    //Start setting values in the textureinfo struct
    //Note that PNGs -can- have alpha.
    t->width = png_get_image_width(png_ptr,info_ptr);
    t->height = png_get_image_height(png_ptr,info_ptr);
    switch (png_get_color_type(png_ptr,info_ptr)) {
        case PNG_COLOR_TYPE_RGBA:
            t->hasAlpha = true;
            break;
        case PNG_COLOR_TYPE_RGB:
        	t->hasAlpha = false;
            break;
        default:
            printf("Invalid PNG.\n");
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            exit(1);
    }
    //How long is a row?
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    //Allocate some space!
    t->textureData = (unsigned char*) malloc(row_bytes * t->height);

    //Read the image data into the texture array, in reverse row order. Because GL.
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
    for (int i = 0; i < t->height; i++) {
        memcpy(t->textureData+(row_bytes * (t->height-1-i)), row_pointers[i], row_bytes);
    }

    //Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
    fclose(fp);

    return 0;
}



