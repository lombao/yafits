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
#include "fitsimage.h"



/**********************************************************************/
/* Internal functions                                                 */
uint32_t _FITS_Image_Read_Pixel( TFitsImage * image, int x, int y );
void     _FITS_Image_Write_Pixel( TFitsImage * image, int x, int y, uint32_t val );
TFitsImage * __FITS_Image_Duplicate( TFitsImage * image );
uint32_t __FITS_recursive_clean_star ( TFitsImage * image, int x, int y );
void __FITS_Image_measure_center (  TFitsImage * image );



/**********************************************************************/
/* This is useful for cases like the FITS images downloaded from
 * COAST Telescope of Open University */
void FITS_IMAGE_crop_image(  TFitsImage * image ) {
	
	
	/* check first column and find how many consecutive 0 at the end */
	int i;
	switch ( image->hdu.bitpix ) {
			case 8: 	i = image->hdu.naxis1-1;
						while( image->payload[i] == 0 ) { i--; }
						break;
			case 16:	i = ( image->hdu.naxis1*( image->hdu.bitpix / 8 ) ) - 2;
						while( *(uint16_t *)&image->payload[i] == 0 ) { i--; i--; }
						break;
			default: 
					fprintf(stderr,"Internal Error:: Unexpected BIT Depth: %d",image->hdu.bitpix);
					exit(EXIT_FAILURE);
					break;
	}
	

	
    /* check the last lines to see if they can be cropped as well */
    /* we assume the origin of coordenates bottom left */
    uint32_t f = 0;  int j = 0;
    while ( f < image->hdu.naxis1 ) {
		f = 0; j++;
		int p = image->hdu.naxis1 * ( image->hdu.naxis2-j) * ( image->hdu.bitpix / 8 );
		for(int k=p; k < p+(image->hdu.naxis1*( image->hdu.bitpix / 8 )); k++) { f += image->payload[k]; } 
	}
	j = image->hdu.naxis2 - (j-1);
	

		
	uint8_t * newpayload = (uint8_t *) malloc ( (sizeof(uint8_t)) * j* i  );
	if ( newpayload == NULL )  {
		fprintf(stderr,"Error:: Out of memory. We could not accomodate memory for an crop image operation\n");
		exit(EXIT_FAILURE);
	}
	
	int c = 0;
	for(int y=0;y<j;y++) {
		for(int x=0;x<i;x++) {
			newpayload[c++] = image->payload[ (image->hdu.naxis1 * y * ( image->hdu.bitpix / 8 )) + x ];
		}
	}
	
	image->hdu.naxis1 = i / ( image->hdu.bitpix / 8 );
	image->hdu.naxis2 = j;
      
    /*free(image->payload); */
    image->payload = newpayload;
    
    /* I do not know why this free produces a segmentation fault */ 
	/* free(oldpayload); */

}



void FITS_IMAGE_print_data(  TFitsImage * image ) {
	
	printf("***********************************************\n");
	printf("Number of headers: %d\n",image->nhdus);
	printf("Resolution: %d x %d \n",image->hdu.naxis1,image->hdu.naxis2);
	printf("Bitx depth: %d\n",image->hdu.bitpix); 
	printf("Pixel Avg Value: %d\n",image->pixelavg);
	printf("Pixel Max Value: %d\n",image->pixelmax);
	printf("Flip orientation %s\n",image->hdu.flipstat);
	printf("Exposure %d\n",image->hdu.exptime);
	printf("Number of stars in image detected: %d\n",image->nstars);
	printf("Star Center Lux: %d\n",image->starcenterlux);
	printf("Star Center Max: %d\n",image->starcentermax);
	
	
	
}

void FITS_IMAGE_Pixel_Stats( TFitsImage * image ) {
	
	uint32_t k = 0;
	uint64_t t = 0;
	uint64_t m = 0;
	
	switch( image->hdu.bitpix ) { 
		
		case 8:		for(k=0; k < image->hdu.naxis1 * image->hdu.naxis1 ; k++) {
						t += image->payload[ k ];
						if ( image->payload[ k ]  > m ) { m = image->payload[ k ]; }
					}
					break;
		case 16:	for(k=0; k < image->hdu.naxis1 * image->hdu.naxis1 * 2; k+=2) {
						t += *(uint16_t *)&image->payload[ k ];
						if ( *(uint16_t *)&image->payload[ k ] > m ) { m = *(uint16_t *)&image->payload[ k ]; }
					}
					break;
		default: 	fprintf(stderr,"Internal Error:: Unexpected BIT Depth: %d",image->hdu.bitpix);
					exit(EXIT_FAILURE);
					break;
	}
	
	
	image->pixelmax = m;
	image->pixelavg = (int)((double)t /  (double)(image->hdu.naxis1 * image->hdu.naxis1));
	
	image->nstars = FITS_Image_star_count ( image ); 
	__FITS_Image_measure_center (  image );
	
	
}




/**********************************************************************/

int FITS_Image_star_count ( TFitsImage * image ) {
	
	TFitsImage * newimage =  __FITS_Image_Duplicate( image );
	
	
	int starcount = 0;
	for( int y=0; y<newimage->hdu.naxis2 ; y++ ) {
		for( int x=0; x<newimage->hdu.naxis1 ; x++ ) {
			uint32_t p = _FITS_Image_Read_Pixel( newimage,x,y);
			if ( p > newimage->pixelavg ) {
				__FITS_recursive_clean_star ( newimage, x, y ); 
				starcount++;
			}
		
		}
	}
	
	
	free(newimage->payload);
	free(newimage);

	return starcount;
}

uint32_t __FITS_recursive_clean_star ( TFitsImage * image, int x, int y ) {
	
	uint32_t p;
	uint32_t k;
	
	p = _FITS_Image_Read_Pixel( image, x, y );
    k = p;
    
    	
	if ( p > image->pixelavg ) {
	
		_FITS_Image_Write_Pixel(image,x,y,0);
		
		if ( x+1 < image->hdu.naxis1 )  							{ k += __FITS_recursive_clean_star( image, x+1,y); }
		if ( x -1 > 0  )                							{ k += __FITS_recursive_clean_star( image, x-1,y); }
		if ( y+1 < image->hdu.naxis2 )  							{ k += __FITS_recursive_clean_star( image, x,y+1); }
		if ( y -1 > 0  )                							{ k += __FITS_recursive_clean_star( image, x,y-1); }
		
		if ( x+1 < image->hdu.naxis1 && y-1 > 0 )  					{ k += __FITS_recursive_clean_star( image, x+1,y-1); }
		if ( x+1 < image->hdu.naxis1 && y+1 < image->hdu.naxis2  )  { k += __FITS_recursive_clean_star( image, x+1,y+1); }
		if ( x-1 > 0 && y+1 < image->hdu.naxis2 )  					{ k += __FITS_recursive_clean_star( image, x-1,y+1); }
		if ( x-1 > 0 && y -1 > 0  )                					{ k += __FITS_recursive_clean_star( image, x-1,y-1); }

	}
	else {
		 k = 0;
	 }
	 
	return k;
	
}

/**********************************************************************/
void __FITS_Image_measure_center (  TFitsImage * image ) {
	
	
	
	TFitsImage * newimage =  __FITS_Image_Duplicate( image );
	
	int x = image->hdu.naxis1/2;
	int y = image->hdu.naxis2/2;
	
	uint32_t p = 0;
	uint32_t r = 0;
	int coordx = 0,coordy = 0;

	#define AROUND 60

    for ( int z = y-AROUND; z < y+AROUND ; z ++ ) {
		for ( int h = x-AROUND; h < x+AROUND ; h ++ ) {				
			p = _FITS_Image_Read_Pixel( newimage,h,z);
			if ( p > r ) { coordx = h ; coordy = z ; r = p ; }
		}
	}
			

	image->starcenterlux =  __FITS_recursive_clean_star ( newimage, coordx,coordy );
	image->starcentermax = r;
}


/**********************************************************************/
/* returns a new image duplicating the original */
TFitsImage * __FITS_Image_Duplicate( TFitsImage * image ) {
	
	
	TFitsImage * newimage = (TFitsImage *) malloc( sizeof(TFitsImage) * 1 );
	if ( newimage == NULL ) {
		fprintf(stderr,"Error:: Out of memory trying to allocate memory for an image copy\n");
		exit(EXIT_FAILURE);
	}
	
	memcpy(newimage,image,sizeof(TFitsImage));
	
	newimage->payload = (uint8_t *) malloc ( sizeof(uint8_t) * image->hdu.bitpix/8 * image->hdu.naxis1 * image->hdu.naxis2  );
	memcpy(newimage->payload,image->payload, sizeof(uint8_t) * image->hdu.bitpix/8 * image->hdu.naxis1 * image->hdu.naxis2  );
		
		
	return newimage;
}








/**********************************************************************/
uint32_t _FITS_Image_Read_Pixel( TFitsImage * image, int x, int y ) {
	
	
	uint32_t p;
	
	
	switch( image->hdu.bitpix ) {
		case 8: p = image->payload [ y * image->hdu.naxis1 + x ];
				break; 
		case 16: p = *(uint16_t *)&image->payload [ y*image->hdu.naxis1*2 + x*2 ];
				break;
		default: 	fprintf(stderr,"Internal Error:: Unexpected BIT Depth: %d",image->hdu.bitpix);
					exit(EXIT_FAILURE);
					break;	
	}
	
	return p;
	
}


void _FITS_Image_Write_Pixel( TFitsImage * image, int x, int y, uint32_t val ) {
	
		
	switch( image->hdu.bitpix ) {
		case 8: image->payload [ y * image->hdu.naxis1 + x ] = (uint8_t)val;
				break; 
		case 16: *(uint16_t *)&image->payload [ y*image->hdu.naxis1*2 + x*2 ] = (uint16_t)val;
				break;
		default: 	fprintf(stderr,"Internal Error:: Unexpected BIT Depth: %d",image->hdu.bitpix);
					exit(EXIT_FAILURE);
					break;	
	}
	
	
}
