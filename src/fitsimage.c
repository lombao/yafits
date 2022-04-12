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
 
 

 #define _GNU_SOURCE
#include "fits.h"
#include "fitsimage.h"



/**********************************************************************/
/* This is useful for cases like the FITS images downloaded from
 * COAST Telescope of Open University */
void FITS_IMAGE_crop_image(  TFitsImage * image, char * resize, const char * filename ) {
	
	
	/* there is a flow in this. It does the crop independently
	 * in each channel and then set the image resolution based on the
	 * results of the last channel */
	
		
	int a=0;
	while( resize[a] != 0x0 && resize[a] != 'x' ) { a++; }
	if ( resize[a]==0x0 ) { 
     		fprintf(stderr,"Error:: The resize parameter for crop is incorrect: It must be in the form <number>x<number>: .i.e: 1200x800 \n");
			exit(EXIT_FAILURE);
	}
	resize[a]=0x0;
	int newsizex = atoi( resize );
	int newsizey = atoi( &resize[a+1] );
	
	if ( newsizex > image->resx || newsizey > image->resy ) {
		     fprintf(stderr,"Error:: The resize parameter must be equal or less than the current image resolution\n");
			exit(EXIT_FAILURE);
	}
	
	for(int cx=0;cx<image->nchannels;cx++) {
	
		
		uint16_t * newpayload = (uint16_t *) malloc ( (sizeof(uint16_t)) * newsizex * newsizey  );
		if ( newpayload == NULL )  {
			fprintf(stderr,"Error:: Out of memory. We could not accomodate memory for an crop image operation\n");
			exit(EXIT_FAILURE);
		}
	
		int c = 0;
		for(int y=0;y<newsizey;y++) {
			for(int x=0;x<newsizex;x++) {
				newpayload[c++] = image->channel[cx][ (image->resx * y) + x ]  ;
			}
		}
	
      
    /*free(image->payload); */
		image->channel[cx] = newpayload;
    
    /* I do not know why this free produces a segmentation fault */ 
	/* free(oldpayload); */
	}
	
	image->resx = newsizex;
	image->resy = newsizey;
	
	
    char newfile[512];
	strncpy(newfile,basename(filename),512);
	newfile[500]=0x0;
	strcat(newfile,".new");
	FILE * fp = fopen (newfile, "wb");  
    if (fp == NULL ) {
		fprintf(stderr,"Error:: We cannot write this file. Permissions problem ?:: %s\n",newfile);
		exit(EXIT_FAILURE);
    }
    
    char line[FITS_HEADER_LINE_SIZE+1];
    char newval[FITS_HEADER_LINE_SIZE - 9];
    char header[FITS_HEADER_LINE_SIZE+1];
    int k=0;
    for(int nk=0;nk<image->numkeys;nk++) {
		if ( strcmp(image->key[nk],"NAXIS1") == 0 ) {
			sprintf(newval," %d",image->resx);
			sprintf(line,"%-8s= %s",image->key[nk], newval);
		}
		else {
			if ( strcmp(image->key[nk],"NAXIS2") == 0 ) {
				sprintf(newval," %d",image->resy);
				sprintf(line,"%-8s= %s",image->key[nk], newval);
			}
			else {
				sprintf(line,"%-8s= %s",image->key[nk],image->value[nk]);
			}
		}
		
		sprintf(header,"%-80s",line);
		fwrite(header, sizeof(char), FITS_HEADER_LINE_SIZE, fp);
		k++;
	}
	
	int j = (k*FITS_HEADER_LINE_SIZE) / FITS_HEADER_UNIT_CHUNK;
	if ( (k*FITS_HEADER_LINE_SIZE) % FITS_HEADER_UNIT_CHUNK ) { j++; }
	
	for(int a=(k*FITS_HEADER_LINE_SIZE); a<FITS_HEADER_UNIT_CHUNK*j;a++) {
		fwrite(" ", sizeof(char), 1, fp);
	}
	
	for(int cx=0;cx<image->nchannels;cx++) {
		for ( int by=0;by<image->resx*image->resy;by++ ) {
			uint16_t k =  image->channel[cx][by] - image->bzero;
			
			fwrite( ((uint8_t *)&k+1), sizeof(uint8_t), 1, fp);
			fwrite( (uint8_t *)&k, sizeof(uint8_t), 1, fp);
		}

	}
	fclose(fp);
}



void FITS_IMAGE_Pixel_Stats( TFitsImage * image ) {
	
	uint32_t k = 0;
	uint64_t t = 0;
	uint64_t m = 0;
	
	
		for(int cx=0;cx<image->nchannels;cx++) {
			
			for(k=0; k < image->resx * image->resy;  k++) {
					t += image->channel[cx][ k ];
					if ( image->channel[cx][ k ] > m ) { m = image->channel[cx][ k ]; }
			}
		
			image->pixelmax[cx] = m;
			image->pixelavg[cx] = (int)((double)t /  (double)(image->resx * image->resy));

		}
		
}


/**********************************************************************/
/**********************************************************************/
/*
int FITS_Image_star_count ( TFitsImage * image ) {
	
		for(cx=0;cx<image->nchannels;cx++) {
			
			newchannel = (uint16_t *) malloc ( sizeof(uint16_t) * image->resx * image->resy  );
			memcpy(newimage,image->channel[cx], sizeof(uint16_t)  * image->resx * image->resy );
			
			
			int starcount = 0;
			for( int y=0; y< image->resy ; y++ ) {
				for( int x=0; x< image->resx ; x++ ) {
					uint32_t p = newchannel[ y * image->resx + x ];
					if ( p > newimage->pixelavg ) {
						__FITS_recursive_clean_star ( newimage, x, y ); 
						starcount++;
					}
		
				}
			}
			
			
		} 
		

	
}
*/






/**********************************************************************/
/**********************************************************************/
/*
int FITS_Image_star_count ( TFitsImage * image ) {
	
	TFitsImage * newimage =  __FITS_Image_Duplicate( image );
	
	
	int starcount = 0;
	for( int y=0; y<newimage->hdu.naxis2 ; y++ ) {
		for( int x=0; x<newimage->hdu.naxis1 ; x++ ) {
			uint32_t p = FITS_Image_Read_Pixel( newimage,x,y);
			if ( p > image->pixelavg ) {
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
	
	p = FITS_Image_Read_Pixel( image, x, y );
    k = p;
    
    	
	if ( p > image->pixelavg ) {
	
		FITS_Image_Write_Pixel(image,x,y,0);
		
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

*/


/**********************************************************************/
/* returns a new image duplicating the original */
/*
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


*/





/**********************************************************************/
/*
uint32_t FITS_Image_Read_Pixel( TFitsImage * image, int x, int y ) {
	
	
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


void FITS_Image_Write_Pixel( TFitsImage * image, int x, int y, uint32_t val ) {
	
		
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
*/
