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
	
	int initdatapos=FITS_HEADER_UNIT_CHUNK * image->nhdus;
	int c = 0;
     
	image->payload = (uint8_t *) malloc ( (sizeof(uint8_t)) * (image->file_size - (FITS_HEADER_UNIT_CHUNK*image->nhdus) )  );
	
	uint16_t w;
	int a = initdatapos;
	
	while( a < image->file_size ) {
		switch ( image->hdu.bitpix ) {
			case 8:	image->payload[c++] = image->buffer[a++] * image->hdu.bscale + image->hdu.bzero;
					    break;
			case 16:  w = (uint16_t)((uint16_t)image->buffer[a] << 8 | image->buffer[a+1]) * image->hdu.bscale + image->hdu.bzero;
						*((uint16_t *) &(image->payload[c]) ) = w;
						a++ ; a++; c++; c++;
						break;
		    case 32:  	fprintf(stderr,"Unsupported:: 32 bits depth is not supported yet, sorry. Contact the author if you would like this feature implemented \n" );
						exit(EXIT_FAILURE);	
						break;
			default:    fprintf(stderr,"Unsupported:: This Bit Depth: %d is not supported yet, sorry. Contact the author if you would like this feature implemented \n",  image->hdu.bitpix);
						exit(EXIT_FAILURE);	
						break;
		}
	}

}
