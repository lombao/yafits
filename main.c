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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h> 

#include "fits.h"
#include "fitsfile.h"
#include "fitsheader.h"
#include "fitsdata.h"
#include "fitsimage.h"
#include "topng.h"


#define YAFITSL_VERSION "0.0.1"

#define MAX_SIZE_FILE_NAME 512

// LOCAL DEFINED FUNCTIONS
void showUsage();



//----------------------------------------------------------------------
/* Print usage */
void showUsage() {
	  
	 fprintf(stderr,"Yet Another FITS Library. Version %s\n",YAFITSL_VERSION); 
	 fprintf(stderr,"Usage: yafitsl [-h] [-H] [-I] [-O] <filename>  \n");
	 fprintf(stderr,"	-h:  Show Help ( this help )\n");
	 fprintf(stderr,"	-H:  Dump headers\n");
	 fprintf(stderr,"	-I:  Print image characteristics\n");
	 fprintf(stderr,"	-O:  Convert to a png file\n");	 
	 fprintf(stderr,"	<file>:  Upload fits file\n");
	 
}





//======================================================================
// * MAIN * 
int main(int argc, char *argv[]) {


  int opt;
  int optionH=0; int optionO=0; int optionI = 0;
  char filename[MAX_SIZE_FILE_NAME];
  char pngfilename[MAX_SIZE_FILE_NAME+5];
  
  TFitsImage * image;
  
  	
	while ((opt = getopt(argc, argv, "hHIO")) != -1) {
        switch (opt) {
        case 'H':
			optionH = 1;     	     
            break;  
        case 'O':  
            optionO = 1;
            break;     
        case 'I':  
            optionI = 1;
            break;      
        case 'h': /* Show version and quit */
            showUsage();
            exit(EXIT_SUCCESS);
            break;
        case 'v': /* Show version and quit */
            showUsage();
            exit(EXIT_SUCCESS);
            break;    
        default: /* '?' */
            showUsage();
            exit(EXIT_FAILURE);
        }
  }
  
  if (argc==optind) { 
	 fprintf(stderr, "\n    ERROR::: No fits image file given \n\n");
	 showUsage();
     exit(EXIT_FAILURE);
  } 
  
  /* Let's get the filename */
  strncpy(filename,argv[optind],MAX_SIZE_FILE_NAME);
  filename[MAX_SIZE_FILE_NAME-1] = 0x0;
  

  image = FITS_File_Image_Upload(filename);
  FITS_Header_Parse ( image );
  FITS_Data_Parse(  image );
  FITS_IMAGE_crop_image(  image );
  FITS_IMAGE_Pixel_Stats( image );
  
  
  
  if ( optionH ) {
	FITS_HEADERS_Print( image );
  }
  
  if ( optionO ) {

	  strcpy(pngfilename,filename);
	  strcat(pngfilename,".png");
	  FITS_topng ( image , pngfilename);
  }
  
  if ( optionI ) {
	FITS_IMAGE_print_data( image );
  }

}
