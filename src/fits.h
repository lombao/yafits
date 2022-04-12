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
 


#ifndef _YAFITSL_H
#define _YAFITSL_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>





/* FITS sizes of HDU blocks */
#define FITS_HEADER_UNIT_CHUNK 2880
#define FITS_HEADER_LINE_SIZE  80

/* This is the limit we set to the number of consecutive HDU's */
#define FITS_MAX_NUM_HDU 10

#define FITS_MAX_SIZE_KEY_STRING 9

#define FITS_MAX_NUM_KEYS 100


/* The max of Channels */
#define FITS_MAX_CHANNELS 20


/*
 * See: https://fits.gsfc.nasa.gov/rfc4047.txt
 * https://archive.stsci.edu/fits/users_guide/
 */
 
typedef struct {
	char simple;
	uint8_t bitpix;
	uint32_t bzero;
	uint8_t bscale;
	uint8_t xbinning;
	uint8_t ybinning;
	uint8_t ravel;
	uint8_t decvel;
	uint32_t exptime;
	uint32_t exposure;
	uint16_t velignt;
	double altitude;
	double azimuth;
	double ha;
	double ra;
	double dec;
	double dectel;
	double ratel;	
	double priority;
	int blank;
	char fullsize[FITS_HEADER_LINE_SIZE];	
	char epoch[FITS_HEADER_LINE_SIZE];
	char extend[FITS_HEADER_LINE_SIZE];
	char frameid[FITS_HEADER_LINE_SIZE];
	char istracki[FITS_HEADER_LINE_SIZE];
	char radecsys[FITS_HEADER_LINE_SIZE];
	char objctra[FITS_HEADER_LINE_SIZE];
	char objctdec[FITS_HEADER_LINE_SIZE];
	char raref[FITS_HEADER_LINE_SIZE];
	char decref[FITS_HEADER_LINE_SIZE];
	char progname[FITS_HEADER_LINE_SIZE];
	char imagetype[FITS_HEADER_LINE_SIZE];
	char filter[FITS_HEADER_LINE_SIZE];	
	char flipstat[FITS_HEADER_LINE_SIZE];		
	char author[FITS_HEADER_LINE_SIZE];
	char origin[FITS_HEADER_LINE_SIZE];
	char telescop[FITS_HEADER_LINE_SIZE];
	char object[FITS_HEADER_LINE_SIZE];
	char dateobs[FITS_HEADER_LINE_SIZE];
	char date[FITS_HEADER_LINE_SIZE];	

	float equinox;
	
} TFitsImageHeader; 




typedef struct {

	
	TFitsImageHeader hdu;
	
    	int headerblocks; /* each block is 2880 bytes */
		int numkeys;
		char * key[FITS_MAX_NUM_KEYS];
		char * value[FITS_MAX_NUM_KEYS];
	
	
	uint32_t file_size;     /* size of the file */
	uint8_t * buffer; 		/* original content from file */
	
	uint8_t nchannels;		/* Number of channels, this is the NAXIS3 value if NAXIES = 3, otherwise is 1 */
	uint16_t * channel[FITS_MAX_CHANNELS]; 	/* The different "images" , there can be more than 1 */
	
	uint32_t pixelavg[FITS_MAX_CHANNELS];
	uint32_t pixelmax[FITS_MAX_CHANNELS];
	
	uint16_t resx; /* This is naxis1 */ 
	uint16_t resy; /* This is naxis2 */
	
	uint8_t bitpix;
	uint32_t bzero;  /* The value that is zero physical_values = BZERO + BSCALE × array_value */
	uint8_t bscale;  /* The bscale https://archive.stsci.edu/fits/fits_standard/node40.html#eq:bscl */

	
} TFitsImage;

 

#endif
