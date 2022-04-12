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
$ yafits -h

Yet Another FITS Library. Version 0.0.6
Usage: yafits [-h|--help] [-e|--export <format>] [-H|--headers] [-S|--stars]  <fitsfile>  
	[-H|--headers]        :  Dump all the header keys of the primary Header Unit
	[-h|--help]           :  Show this Help 
	[-e|--export]         :  Export the image into PNG16 bits RGB 
	[-c|--crop] <sizex>x<sizey> :  Crop the image to sizex by sizey ( .i.e: 1340x1340 ) 
	<fitsfile>            :  The files file to process

```


### To export a FITS image into a PNG image
```
./yafits --export tmp/340078.fits
```

This command will create the file "340078.fits.png" 


### To show all header Keys

```
./yafits --headers 340078.fits
```

### To crop the FITS image to a new size

```
./yafits --crop 1300x1300 340078.fits

```

This will not modify the original file, but 
it will create a new file named 340078.fits.new
Rename it if you are happy with the result

