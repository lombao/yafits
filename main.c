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
#include <getopt.h>
#include <unistd.h> 

#include "fits.h"
#include "fitsfile.h"
#include "fitsheader.h"
#include "fitsdata.h"
#include "fitsimage.h"
#include "topng.h"


#define YAFITSL_VERSION "0.0.3"

#define MAX_SIZE_FILE_NAME 512

// LOCAL DEFINED FUNCTIONS
void showUsage();



//----------------------------------------------------------------------
/* Print usage */
void showUsage() {
	  
	  
	 fprintf(stderr,"\nYet Another FITS Library. Version %s\n",YAFITSL_VERSION); 
	 fprintf(stderr,"Usage: yafits [-h|--help] [-e|--export <format>] [-H|--headers] [-S|--stars]  <fitsfile>  \n");
	 fprintf(stderr,"	[-h|--help]           :  Show this Help \n");
	 fprintf(stderr,"	[-e|--export] <format>:  Export the image into one of these formats: [ png ]\n");
	 fprintf(stderr,"	[-H|--headers]        :  Dump all the header keys of the primary Header Unit\n");
	 fprintf(stderr,"	[-S|--stars]          :  Count how many starts in the picture\n"); 
	 fprintf(stderr,"	<fitsfile>            :  The files file to process\n");
	 
	 	 fprintf(stderr,"\n\n");
}





//======================================================================
// * MAIN * 
int main(int argc, char *argv[]) {


  int opt;

  char filename[MAX_SIZE_FILE_NAME];

  
  #define MAX_EXPORT_FORMAT_NAME 10
  char exportFormat[MAX_EXPORT_FORMAT_NAME];
  
  TFitsImage * image;
  
  int flagHelp = 0;	
  int flagExport = 0;
  int flagHeaders = 0;
  int flagStars = 0;
  	
  struct option longopts[] = {
   { "export",      required_argument,		NULL,   	'e'   },
   { "help",    	no_argument,       		NULL,    	'h'  },
   { "headers",    	no_argument,       		NULL,    	'H'  },
   { "stars",    	no_argument,       		NULL,    	'S'  },
   
   { 0, 0, 0, 0 }
  };
  	
  	
	while ((opt = getopt_long(argc, argv, "HShe:",longopts,NULL)) != -1) {
        switch (opt) {
   
        case 'h': /* Show Help & version and quit */
            flagHelp = 1;
            break;
            
        case 'H': /* Dump Headers */
            flagHeaders = 1;
            break;
            
        case 'S': /* Dump Headers */
            flagStars = 1;
            break;
         
        case 'e': /*Export */
			flagExport = 1;
			strncpy(exportFormat,optarg,MAX_EXPORT_FORMAT_NAME);
			exportFormat[MAX_EXPORT_FORMAT_NAME-1]=0x0;
			break;
				
		case ':':   /* missing option argument */
			fprintf(stderr, "ERROR: option `-%c' requires an argument\n", optopt);
			exit(EXIT_FAILURE);
			break;
		
		
		
		case '?':
		default:    /* invalid option */
			fprintf(stderr, "ERROR: option `-%c' is invalid: ignored\n", optopt);
			exit(EXIT_FAILURE);
			break;
        
	  }
  }
  
  /* If help, then show and quit */
  if ( flagHelp ) {
	  showUsage();
	  exit(EXIT_SUCCESS);
  }
  
 
  /* If not file given, then quit */
  if (argc==optind) { 
	 fprintf(stderr, "ERROR::: No fits image file given \n\n");
     exit(EXIT_FAILURE);
  } 
  
  /* Let's get the filename */
  strncpy(filename,argv[optind],MAX_SIZE_FILE_NAME);
  filename[MAX_SIZE_FILE_NAME-1] = 0x0;
  

  /* Upload and pre-process the image */
  image = FITS_File_Image_Upload(filename);
  FITS_Header_Parse ( image );
  FITS_Data_Parse(  image );
  FITS_IMAGE_crop_image(  image );
  FITS_IMAGE_Pixel_Stats( image );
  
  
  
  if ( flagHeaders ) {
	  FITS_HEADERS_Print( image );
  }
  
  
  if ( flagStars ) {
	printf("\nSTARS\n");
	printf("==============================\n");
	printf("There are %d stars \n",FITS_Image_star_count ( image ));
  }
  
  
  if ( flagExport ) {

	  if ( strncmp(exportFormat,"png",strlen("png")) == 0 ) { 
		FITS_topng ( image , filename);
	  }
	  else {
		fprintf(stderr, "ERROR::: Export format : %s not supported\n",exportFormat);
		exit(EXIT_FAILURE);
	  }
  }
  


}
