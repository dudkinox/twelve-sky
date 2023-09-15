#****************************************************************************
#
##   CTSD : Common Transaction Service
#
##   CopyRight(C) 2003 Sean Kim All Rights reserved.
#   Author : Sean Kim (sean@0x31.com)
#   #
#   $Id: Makefile,v 1.1 2007/06/29 08:40:00 sean Exp $
#   #
#   Redistribution and use in source and binary forms, with or with out
#   #   modification, are not permitted without author's permission.
#
##****************************************************************************

DIRS = src
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
