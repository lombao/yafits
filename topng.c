/* Copyright (c) 2022 Cesar Lombao
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
 
 

#include <png.h>
 
#include "fits.h"
#include "topng.h"



/* Data structures to build the PNG */        
typedef struct  {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Tpixel;
    
typedef struct {
    Tpixel *pixels;
    size_t width;
    size_t height;
} Tbitmap;

    
   
 
/*********************************************************************/      
        

void FITS_topng ( TFitsImage * image , const char * filename ) {
	
    Tbitmap bm;
    size_t x, y;
    int xx,yy;

    /* Create . */
	bm.height = image->hdu.naxis2;
	bm.width  = image->hdu.naxis1;
    bm.pixels = malloc( (sizeof (Tpixel)) * bm.height * bm.width );
    if ( bm.pixels == NULL ) {
		fprintf(stderr,"Error:: Out of memory. We could not accomodate memory for the PNG data");
		exit(EXIT_FAILURE);
    }

    int k= 0;
	/* Building the bitmap */
    /* Note, assumes the FITS Image origin of coordiantes is bottom left
     * instead the top left */ 
    for (yy = bm.height; yy >= 0 ; yy--) {
        for (xx = 0; xx < bm.width; xx++) {
            Tpixel * pixel = bm.pixels + ( bm.width * yy + xx );

            int v;
            switch( image->hdu.bitpix ) {
				case 8: v =  image->payload[k]; 
						k++;
						break;
				case 16: v = (int) ( ( ((double)*((uint16_t *)(&image->payload[k]))) / 65535.0) *  256.0);
						k++ ; k++;
						break;
				default: 	fprintf(stderr,"Internal Error:: Unexpected BIT Depth: %d",image->hdu.bitpix);
							exit(EXIT_FAILURE);
							break;
			}

            pixel->red =   v;
            pixel->green = v;
            pixel->blue =  v;
        }
    }

    /******************************************************************/
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte ** row_pointers = NULL;


    int pixel_size = 3;
    int depth = 8;
    
	FILE * fp = fopen (filename, "wb");  
    if (fp == NULL ) {
		fprintf(stderr,"Error:: We cannot write this file. Permissions problem ?:: %s\n",filename);
		exit(EXIT_FAILURE);
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);    
    info_ptr = png_create_info_struct (png_ptr);

       
    /* Set image attributes. */
    png_set_IHDR (png_ptr,
                  info_ptr,
                  bm.width,
                  bm.height,
                  depth,
                  PNG_COLOR_TYPE_RGB,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);
    
    /* Initialize rows of PNG. */
    
    row_pointers = png_malloc (png_ptr, bm.height * sizeof (png_byte *));
    for (y = 0; y < bm.height; y++) {
        png_byte *row = 
            png_malloc (png_ptr, sizeof (uint8_t) * bm.width * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < bm.width; x++) {
            Tpixel * pixel = bm.pixels + bm.width * y + x;
            *row++ = pixel->red;
            *row++ = pixel->green;
            *row++ = pixel->blue;
        }
    }
    
    /* Write the image data to "fp". */

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */

    
    for (y = 0; y < bm.height; y++) { 
		png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);  
    free (bm.pixels);
	
}
