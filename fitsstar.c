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
 

#include "fitsimage.h"
#include "fitsstar.h" 
 
 
/**********************************************************************/
/* Given an image it will try to locate the brightest start around the 
 * center of the image */
void FITS_Star_spot_center (  TFitsImage * image, int * thex, int * they ) {
		
	int x = image->hdu.naxis1/2;
	int y = image->hdu.naxis2/2;
	
	uint32_t p = 0;	uint32_t r = 0;
	int coordx = 0,coordy = 0;

	#define AROUND 80

    for ( int z = y-AROUND; z < y+AROUND ; z ++ ) {
		for ( int h = x-AROUND; h < x+AROUND ; h ++ ) {				
			p = FITS_Image_Read_Pixel( image,h,z);
			if ( p > r ) { coordx = h ; coordy = z ; r = p ; }
		}
	}
			

	*thex = coordx;
	*they = coordy;

}
