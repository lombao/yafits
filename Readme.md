# YAFITS (  Yet Another FITS package )

This is a "naive" attempt to built my own FITS package to process 
astrophotography images in my Linux box. 

This is a very much work in progress, if you want to make any suggestions
please do so at cesar dot lombao at gmail dot com

The inspiration to build this came from the fantastic OU course "Astronomy
with an online telescope:
https://www.open.edu/openlearn/science-maths-technology/astronomy/astronomy-online-telescope". 
I cannot praise enough OpenUniversity for such fantastic free resource.




## Compiling

Just clone the repo and:

```
git clone https://github.com/lombao/yafits.git
cd yafits
./autogen.sh
./configure
./make ; sudo make install
```

If you want install in a different place than /usr/local just use "prefix"
in the configure

```
./configure --prefix=/yourpreferredlocation
```


## How to use it

```
[lombao@fedora yafits]$ yafits -h

Yet Another FITS Library. Version 0.0.4
Usage: yafits [-h|--help] [-e|--export <format>] [-H|--headers] [-S|--stars]  <fitsfile>  
	[-h|--help]           :  Show this Help 
	[-e|--export] <format>:  Export the image into one of these formats: [ png ]
	[-H|--headers]        :  Dump all the header keys of the primary Header Unit
	[-S|--stars]          :  Count how many starts in the picture
	[-C|--spotcenterstar] :  Gives the coordinates of the brightest star closest to the center of the image
	<fitsfile>            :  The files file to process
```


### To export a FITS image into a PNG image
```
./yafits --export png tmp/340078.fits
```

This command will create the file "340078.fits.png" where the yafits is 
run.


### To show all header Keys

```
./yafits --headers 340078.fits
```

### To count the number of stars

```
./yafits --stars 340078.fits

STARS
==============================
There are 654 stars 

```

### To get the coordinates of the center star

```
[]$ ./yafits --spotcenterstar /tmp/340078.fits

COORDINATES OF CENTER STAR
==============================
The image has a resolution of 1536 , 1536 pixels
There brightest star around the center is at: 764 , 721  ( Cartesian coordinates )
If the image is transferred to PNG or JPG format the coordinates will be: 764 , 815

```
