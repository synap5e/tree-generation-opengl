#include "pngloader.hpp"
#include "string.h"

GLuint load_PNG(const char* filename, TextureInfo* t){

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



