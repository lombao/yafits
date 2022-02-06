# YAFITS
## Yet Another FITS package

This is a "naive" attempt to built my own FITS package to process 
astrophotography images. 

This is a very much work in progress, if you want to make any suggestions
please do so at cesar dot lombao at gmail dot com



## Compiling

Just clone the repo

> make all


## How to use it

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
