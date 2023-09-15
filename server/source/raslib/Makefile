#/****************************************************************************
#
#  Ras library FreeBSD Version
#
#  Copyright(C) 2003 Sean Kim All Rights reserved.
#  Author : Sean Kim (sean@0x31.com)
#
#  $Id: Makefile.in,v 1.1 2007/03/21 06:14:07 miracle Exp $
#
#  Redistribution and use in source and binary forms, with or with out
#  modification, are not permitted without author's permission.
# ****************************************************************************/

CPWD := $(shell pwd)
DIRS = src
BIN_DIR = ../lib
BIN     = $(BIN_DIR)/lras.a

all:
	@for DIR in ${DIRS}; do \
		echo "Entering $${DIR} ..."; \
		(cd $${DIR}; make all); \
		echo "Leaving $${DIR} ..."; \
	done
install:
	(cd src; make install)
clean:
	(cd src; make clean)
	rm -f *.o
	rm -f $(BIN)

# default:
	# $(MAKE) $(BIN)

# $(BIN): $(OBJFILES)
	# if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi
	# ar cru $(BIN) $(OBJFILES) $(LIBS)
	# ranlib $(BIN)
	# chmod 700 $(BIN)
