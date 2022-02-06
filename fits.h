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





/* FITS sizes of HDU blocks */
#define FITS_HEADER_UNIT_CHUNK 2880
#define FITS_HEADER_LINE_SIZE  80

/* This is the limit we set to the number of consecutive HDU's */
#define FITS_MAX_NUM_HDU 10

#define FITS_MAX_SIZE_KEY_STRING 9

#define FITS_MAX_NUM_KEYS 100


/*
 * See: https://fits.gsfc.nasa.gov/rfc4047.txt
 * https://archive.stsci.edu/fits/users_guide/
 */
 
typedef struct {
	char simple;
	uint8_t bitpix;
	uint8_t naxis;
	uint16_t naxis1;
	uint16_t naxis2;
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
	int nhdus;
	
	TFitsImageHeader hdu;
	
	char * keylines[FITS_MAX_NUM_KEYS];
	int numkeys;
	
	uint32_t file_size;     	/* size of the file */
	uint8_t * buffer; 	/* original image frmo file */
	uint8_t * payload; 	/* The data unit */
	
	uint32_t pixelavg;
	uint32_t pixelmax;
	uint32_t nstars;
	
	uint32_t starcenterlux;
	uint32_t starcentermax;
	
} TFitsImage;

 

#endif
