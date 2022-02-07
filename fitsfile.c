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
 
 
#include "fits.h"
#include "fitsfile.h"






TFitsImage * FITS_File_Image_Upload( const char * filename ) {

	FILE * fp;
	TFitsImage * image;
	
	
	image = (TFitsImage *) malloc( sizeof(TFitsImage) * 1 );
	if ( image == NULL ) {
		fprintf(stderr,"Error:: Out of memory trying to allocate the basic blocks\n");
		exit(EXIT_FAILURE);
	}
	
	
	fp = fopen(filename,"rb");
	if ( fp == NULL) {
		fprintf(stderr,"Error:: File not found or not accesible:: %s\n",filename);
		exit(EXIT_FAILURE);
	}
		
	fseek(fp, 0L, SEEK_END);
	image->file_size = ftell(fp);
	rewind(fp);
	
	if ( image->file_size > LIMIT_MAX_FILE_SIZE ) { 
		fprintf(stderr,"Error:: Image is too big and exceeds the internal hardcoded limit ( %d bytes ) \n",LIMIT_MAX_FILE_SIZE);
		exit(EXIT_FAILURE);
	}	
	
	image->buffer = (uint8_t *) malloc( sizeof(uint8_t) * image->file_size+1 );
	if ( image->buffer == NULL )  {
		fprintf(stderr,"Error:: Out of memory. We could not accomodate in memory the size of this file: %d bytes \n",image->file_size);
		exit(EXIT_FAILURE);
	}
	
	
	uint32_t vr = fread(image->buffer,1,image->file_size,fp);
	if ( vr != image->file_size ) {
		fprintf(stderr,"Internal Error:: File Corrupted ?? The number of bytes read from the file: %d, is different from the size in bytes of the file: %d.\n",vr,image->file_size);
		exit(EXIT_FAILURE);	
	}
	
	
	image->pixelavg = 0;
	image->pixelmax = 0;
	
	image->starcenterlux = 0;
	image->starcentermax = 0;
	
	return image;
	
}

