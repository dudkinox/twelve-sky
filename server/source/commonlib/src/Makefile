PRGNAME	= win2unix

# set directories up !

srcdir 		= .


install_prefix	=
prefix		= /usr/local
exec_prefix	= ${prefix}
libdir		= ${exec_prefix}/lib
includedir	= ${prefix}/include

# a version definition of the library

LIBNAME = lib${PRGNAME}.a 

SLIBVERSION		= 1
SLIBNAME		= lib${PRGNAME}.so
SLIBREALNAME	= ${SLIBNAME}.${SLIBVERSION}

# Which Compiler

CXX = g++

# compile definition

DEFS = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_SYS_FILE_H=1 -DHAVE_SYS_IOCTL_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_UNISTD_H=1 -DHAVE_SYS_SELECT_H=1 -DHAVE_SYS_EPOLL_H=1 -DTIME_WITH_SYS_TIME=1 -DSETPGRP_VOID=1 -DRETSIGTYPE=void -DHAVE_VPRINTF=1 -DHAVE_SOCKET=1 -DHAVE_STRDUP=1 -DHAVE_SELECT=1 
 
# Options for release

# static library 
INCLUDES = -I../include -I/usr/local/include 
CFLAGS 	=   -fPIC -shared -rdynamic -Wall -Wimplicit -Wreturn-type -Wunused -Wswitch -Wuninitialized\
			-Wparentheses -Wshadow -Wpointer-arith -Woverloaded-virtual\
			-O3
DFLAGS = 

# Which Library archiver

AR 	= /usr/bin/ar 
ARFLAGS = -rc
LD  = /usr/bin/ld 
LDFLAGS = -G -soname ${PRGNAME}

# utilities

LN_S = ln -s
RM = /bin/rm
CHMOD = /bin/chmod
MKDIRTOOL = ../mkinstalldirs

# install script

INSTALL = /usr/bin/install -c -m 0644

# Which Library Indexer

RANLIB = ranlib

# Source files 

SRC = win2unix.cpp

# Header files
HEADER = win2unix.h

# Objects List	
OBJ = win2unix.o

## Make Library
all : $(OBJ)
	${RM} -f ${LIBNAME}
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJ)
	$(RANLIB) $(LIBNAME)
	$(CXX) ${CFLAGS} -rdynamic -Wl,--export-dynamic -Wl,-soname,${SLIBREALNAME} -o ${SLIBREALNAME} ${OBJ}
#   $(LD) $(LDFLAGS) -o ${SLIBREALNAME} ${OBJ}
#   ${RM} -f ${SLIBNAME}
#   ${LN_S} ${SLIBREALNAME} ${SLIBNAME}

.cpp.o : $(SRC) 
	$(CXX) $(CFLAGS) $(DFLAGS) $(INCLUDES) -c -o $@ $< ${LIB} 

install : all
	${MKDIRTOOL} ${prefix} ${libdir} ${includedir}
	${INSTALL} ../include/*.h  ${includedir}
	${INSTALL} ${LIBNAME} ${libdir}/${LIBNAME}
	${INSTALL} ${SLIBREALNAME} ${libdir}/${SLIBREALNAME}	
	${RM} -f ${libdir}/${SLIBNAME}
	${CHMOD} 0655 ${libdir}/${SLIBREALNAME}
	${LN_S} ${libdir}/${SLIBREALNAME} ${libdir}/${SLIBNAME}
	
clean :
	rm -f $(OBJ) $(LIBNAME) ${SLIBREALNAME} ${SLIBNAME}

