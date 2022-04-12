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
#include "fitsheader.h"


/* To read the Main HDU where all tkey Keys are */
/* Not only parse them into the specific values */
/* also stores a list of the known and unknown keys in this header */



void FITS_Header_Parse(  TFitsImage * image ) {
	
	char key[FITS_MAX_SIZE_KEY_STRING];
	char val[FITS_HEADER_UNIT_CHUNK];
	
	
	/* Defaults */
	image->hdu.bscale = 1;
	image->numkeys = 0;
	image->headerblocks = 0;
	
	/* */
	int flag_end = 0;
	int nheaders = 0;
	int a; 
	while ( flag_end == 0 ) {
		
		for(a=nheaders*FITS_HEADER_UNIT_CHUNK ; a < (nheaders+1)*FITS_HEADER_UNIT_CHUNK; a+=FITS_HEADER_LINE_SIZE) {

			int i = 0;
			strncpy(key,(char *)&image->buffer[a], FITS_MAX_SIZE_KEY_STRING);
			key[FITS_MAX_SIZE_KEY_STRING - 1] = 0x0;
			while(key[i]!=0x0) { if (key[i]==' ') { key[i]=0x0;}  else { i++; } }
		
		
			i = FITS_MAX_SIZE_KEY_STRING;
			while( image->buffer[a+i]==' ') { i++; }
			if ( i > FITS_HEADER_LINE_SIZE ) {
				val[0] = 0x0;
			}
			else {
				strncpy(val,(char *)&image->buffer[a+i],FITS_HEADER_LINE_SIZE-9-i);
				val[FITS_HEADER_LINE_SIZE-FITS_MAX_SIZE_KEY_STRING-i] = 0x0;
				int  h = FITS_HEADER_LINE_SIZE-FITS_MAX_SIZE_KEY_STRING-i-1;
				while(val[h]==' ') { val[h]=0x0; h--; }
			}
			
			
			image->key[ image->numkeys ] =  malloc( sizeof(char) * (strlen(key) + 1) );
			image->value[ image->numkeys ] =  malloc( sizeof(char) * (strlen(val) + 1) );
			
			strcpy( image->key[ image->numkeys ] , key );
			strcpy( image->value[ image->numkeys ] , val );
			
			image->numkeys++;
			
			switch( key[0] ) {

				case 'A': if (strcmp(key,"AUTHOR") == 0 ) { strcpy(image->hdu.author,val); }
						  else {
							  if (strcmp(key,"ALTITUDE") == 0 ) { image->hdu.altitude = atol(val); }
							  else {
								  if (strcmp(key,"AZIMUTH") == 0 ) { image->hdu.azimuth = atol(val); }
								  else {
									printf("Unknown header %s\n",key);
								  }
							  }
						  }
						  break;
						  
				case 'B': if (strcmp(key,"BITPIX") == 0 ) { image->hdu.bitpix = atoi(val); image->bitpix = atoi(val); 
					                                        if ( atoi(val) != 16 ) {  
																                     fprintf(stderr,"Error:: Unsupported BITPX: %d. Sorry. only 16 bits per pixel is supported\n",atoi(val));
					                                                                 exit(EXIT_FAILURE); 
					                                                                } 
					                                      }	
						  else {
					  			if (strcmp(key,"BZERO") == 0 ) { image->bzero = image->hdu.bzero = atoi(val); }
					  			else {
									if (strcmp(key,"BSCALE") == 0 ) {image->bscale =  image->hdu.bscale = atoi(val); }
									else {
										if (strcmp(key,"BLANK") == 0 ) { image->hdu.blank = atoi(val); }
										else {
											 printf("Unknown header %s\n",key);
										}
									}
								}
						  }
						  break;
						  
				case 'D': if (strcmp(key,"DATE-OBS") == 0 ) { strcpy(image->hdu.dateobs,val); }
						  else {
							  if (strcmp(key,"DATE") == 0 ) { strcpy(image->hdu.dateobs,val); }
							  else {
								  if (strcmp(key,"DECVEL") == 0 ) { image->hdu.decvel = atoi(val);  }
								  else {
									  if (strcmp(key,"DEC-REF") == 0 ) { strcpy(image->hdu.decref,val); }
									  else {
										  if (strcmp(key,"DEC") == 0 ) {  image->hdu.dec = atol(val);  }
										  else {
											  if (strcmp(key,"DEC-TEL") == 0 ) {  image->hdu.dectel = atol(val);  }
											  else {									
													printf("Unknown header %s\n",key);
											  }
									      }
									  }
								  }
							  }
						  }
				          break;
				          	
				case 'E': if (strcmp(key,"EXPTIME") == 0 ) { image->hdu.exptime = atoi(val); }
						  else {
							  if (strcmp(key,"EQUINOX") == 0 ) { image->hdu.equinox = atof(val); }
							  else {
								  if (strcmp(key,"END") == 0 ) { flag_end = 1; val[1]=0x0; a =  (nheaders+1)*FITS_HEADER_UNIT_CHUNK; }	
								  else {
									  if (strcmp(key,"EXPOSURE") == 0 ) {  image->hdu.exposure = atoi(val); }
									  else {	
										 if (strcmp(key,"EXTEND") == 0 ) {  strcpy(image->hdu.extend,val); } 
										 else {
											if (strcmp(key,"EPOCH") == 0 ) {  strcpy(image->hdu.epoch,val); } 
											else {
												printf("Unknown header %s\n",key);
											}
										 }
									  }
								  }
							  }
						  }
						  break;
						  
						  						  
				case 'F': if (strcmp(key,"FILTER") == 0 ) { strcpy(image->hdu.filter,val); }
						  else {
							  if (strcmp(key,"FLIPSTAT") == 0 ) { strcpy(image->hdu.flipstat,val); }
							  else {
								  if (strcmp(key,"FRAMEID") == 0 ) { strcpy(image->hdu.frameid,val); }
								  else {
									if (strcmp(key,"FULLSIZE") == 0 ) { strcpy(image->hdu.fullsize,val); }	  
									else {
										printf("Unknown header %s\n",key);  
									}
								  }
							  }
						  }
						  break;
						  
				case 'H': if (strcmp(key,"HA") == 0 ) { image->hdu.ha = atol(val); }
					      else {
							   printf("Unknown header %s\n",key);  
						  }
						  break;
						  
				case 'I': if (strcmp(key,"IMAGETYP") == 0 ) { strcpy(image->hdu.imagetype,val); }
						  else {
							   if (strcmp(key,"ISTRACKI") == 0 ) { strcpy(image->hdu.istracki,val); }
							   else {
									printf("Unknown header %s\n",key);  
							   }
						  }
						  break;
						  
				case 'N': if (strcmp(key,"NAXIS") == 0 ) {  switch  ( atoi(val) ) { case 3: image->nchannels = 3;break; case 2: image->nchannels = 1;break; 
					                                                                default: fprintf(stderr,"Error:: Number of indexes (NAXIS)::%d is not supported, sorry\n",atoi(val));
					                                                                         exit(EXIT_FAILURE); break;
					                                                              }
														}
						  else {
							  if (strcmp(key,"NAXIS1") == 0 ) { image->resx = atoi(val); }
							  else {
								  if (strcmp(key,"NAXIS2") == 0 ) { image->resy = atoi(val); }
								  else {
									  if (strcmp(key,"NAXIS3") == 0 ) { image->nchannels = atoi(val); }
									  else {
									     printf("Unknown header %s\n",key); 
									 }
								  }
							  }
						  }	
						  break;
						  
				case 'O': if (strcmp(key,"ORIGIN") == 0 ) { strcpy(image->hdu.origin,val); }
						  else {
							 if (strcmp(key,"OBJECT") == 0 ) { strcpy(image->hdu.object,val); }
							 else {
								 if (strcmp(key,"OBJCTRA") == 0 ) { strcpy(image->hdu.objctra,val); }
								 else {
									if (strcmp(key,"OBJCTDEC") == 0 ) { strcpy(image->hdu.objctdec,val); }
									else {
										printf("Unknown header %s\n",key);
									}
								}
							 }
						  }
						  break;
						  
				case 'P': if (strcmp(key,"PROGNAME") == 0 ) { strcpy(image->hdu.origin,val); }
						  else {
							 if (strcmp(key,"PRIORITY") == 0 ) { image->hdu.priority = atol(val); }
							 else {
							     printf("Unknown header %s\n",key);
							 }
						  }
						  break;
						  
				
				case 'R': if (strcmp(key,"RAVEL") == 0 ) { image->hdu.xbinning = atoi(val); }
						  else {
							  if (strcmp(key,"RA-TEL") == 0 ) { image->hdu.ratel = atol(val); }
							  else {
								  if (strcmp(key,"RA-REF") == 0 ) {  strcpy(image->hdu.raref,val); }
								  else {
									  if (strcmp(key,"RA") == 0 ) {  image->hdu.ra = atol(val); }				
									  else {
										 if (strcmp(key,"RADECSYS") == 0 ) { strcpy(image->hdu.radecsys,val); }	
										 else {		
											printf("Unknown header %s\n",key);
										 }
									 }
								  }
							  }
						  }
						  break;
				
						  
				case 'S': if (strcmp(key,"SIMPLE") == 0 ) { image->hdu.simple = val[0]; }
						  else {
							     printf("Unknown header %s\n",key);
						  }
						  break;
						  
				case 'T': if (strcmp(key,"TELESCOP") == 0 ) { strcpy(image->hdu.telescop,val); }
						  else {
							     printf("Unknown header %s\n",key);
						  }
							 
						  break;
						  
						  
				case 'V': if (strcmp(key,"VELIGNT") == 0 ) { image->hdu.velignt = atoi(val); }
						  else {
							     printf("Unknown header %s\n",key);
						  }
						  break;
						  
				case 'X': if (strcmp(key,"XBINNING") == 0 ) { image->hdu.xbinning = atoi(val); }
						  else {
							     printf("Unknown header %s\n",key);
						  }
						  break;
						  
				case 'Y': if (strcmp(key,"YBINNING") == 0 ) { image->hdu.ybinning = atoi(val); }
						  else {
							     printf("Unknown header %s\n",key);
						  }
						  break;		  
				default : printf("Unknown header %s\n",key);
						  break;
			} /* end switch */
		
		} /* end for */
		
		nheaders++;
		if ( nheaders >= FITS_MAX_NUM_HDU ) {
			fprintf(stderr,"Unsupported:: This FITS file seems to have more than %d Headers ( HDU ) , or perhaps it is corrupted\n",FITS_MAX_NUM_HDU );
			exit(EXIT_FAILURE);	
		}
		
	} /* end of while */
			
	
	image->headerblocks = nheaders;
	
}


/*********************************************************************/

void FITS_HEADERS_Dump(  TFitsImage * image ) {
	
	printf("\n");
	printf("FITS:             HEADER KEYS\n");
	printf("=============================\n");
	printf("These are the keys found in the header of this image\n");
	printf("Please note that not all belong the to FITS spec\n\n");
	
	for(int a=0;a < image->numkeys; a++ ) {
		
		if ( strncmp(image->key[a],"END",3) == 0 ) {
			printf ("END  ______ \n");
		}
		else {
			printf ("%15s = %-60s\n",image->key[a],image->value[a]);
		}
		
	}
	printf("\n*** TOTAL: %d keys\n\n",image->numkeys);
}
