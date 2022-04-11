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
 
 
#ifndef _FITS_IMAGE_H
#define _FITS_IMAGE_H



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fits.h"


void FITS_IMAGE_crop_image(  TFitsImage * image );
void FITS_IMAGE_Pixel_Stats( TFitsImage * image );


uint32_t FITS_Image_Read_Pixel( TFitsImage * image, int x, int y );
void     FITS_Image_Write_Pixel( TFitsImage * image, int x, int y, uint32_t val );

int FITS_Image_star_count ( TFitsImage * image );

#endif
