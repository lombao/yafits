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
#include "fitsdata.h"



void FITS_Data_Parse(  TFitsImage * image ) {
	
	
	/* https://archive.stsci.edu/fits/users_guide/node21.html#eq:bscal */
	
	int initdatapos=FITS_HEADER_UNIT_CHUNK * image->headerblocks;

     
    /* Each "image" is a channel, possibly with its own filter */
    /* We allocate mem for each one */
    for(int ax=0;ax<image->nchannels;ax++) { 
		image->channel[ax] = (uint16_t *) malloc ( (sizeof(uint16_t)) * image->resx * image->resy * (image->bitpix/16)  );
	}
	
	uint16_t w;
	int a = initdatapos;

	
	for(int ax=0;ax<image->nchannels;ax++) { 	
		for(int px=0;px < (image->resx * image->resy) ; px++ ) { 
			w = (uint16_t)((uint16_t)image->buffer[a] << 8 | image->buffer[a+1]) * image->hdu.bscale + image->hdu.bzero;
			image->channel[ax][px] = w;
			a++ ; a++; /* the buffer image is 8 bits, so we move the pointer twice */	
		}
		
	}
	

}
