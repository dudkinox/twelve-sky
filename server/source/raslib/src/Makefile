PRGNAME	= ras

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
			-O3 -msse -march=nocona -mfpmath=sse -mmmx -msse3
#-O1 -g -msse -march=nocona -mfpmath=sse -mmmx -msse3
DFLAGS = -D__NET_IO_USE_EPOLL__ -D__USE_SEMUN__ -D__x86_64__#-D__i686__
#DFLAGS = -D__NET_IO_USE_EPOLL__ -D__i686__
#DFLAGS = -D__NET_IO_USE_KQUEUE__ 

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

SRC = address.cpp blowfish.cpp bytebuffer.cpp config.cpp log.cpp md5.cpp\
	  net_io.cpp semaphore.cpp socket.cpp stringbuffer.cpp tcpclient.cpp\
	  tcpconnection.cpp tcpserver.cpp thread.cpp udpnode.cpp\
	  util.cpp sysutil.cpp 

# Header files
HEADER = address.h avl.h blowfish.h buffer.h bytebuffer.h exception.h\
		 log.h md5.h net_io.h netint.h option.h raslib.h rconfig.h\
		 mutex.h socket.h stringbuffer.h tcpclient.h tcpconnection.h\
		 tcpserver.h thread.h udpnode.h util.h sysutil.h db_pool.h pointer.h

# Objects List	
OBJ = address.o blowfish.o bytebuffer.o config.o log.o md5.o\
	  net_io.o semaphore.o socket.o stringbuffer.o tcpclient.o\
	  tcpconnection.o tcpserver.o thread.o udpnode.o\
	  util.o sysutil.o

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

