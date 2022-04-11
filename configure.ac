#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([yafits], [0.0.5], [cesar.lombao@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_YACC
AC_PROG_LEX
AM_PROG_AR

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h stdio.h string.h stdarg.h getopt.h unistd.h])

# Check for LIBPNG
PKG_CHECK_MODULES([LIBPNG], libpng)

# Checks for library functions.
AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign])
AC_CONFIG_FILES([src/Makefile Makefile])
LT_INIT([shared disable-static])
AC_OUTPUT

echo "
	YAFITS ${VERSION} has been configured for compilation
	It will be built into PREFIX: ${bindir}
	
	The configuration is:
	  LIBPNG_LIBS: ${LIBPNG_LIBS}
          LIBPNG_CFLAGS: ${LIBPNG_CFLAGS}
          PREFIX: ${prefix}
          VERSION: ${VERSION}          
          	
	Now you can :
	 make
	 sudo make install
	
"
