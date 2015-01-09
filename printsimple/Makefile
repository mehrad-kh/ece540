TARGET =	printsimple
PACKAGE_NAME =	printsimple
SUPER_PACKAGES_PATH = simplesuif
SUB_PACKAGES =
LIBS =		-lsimple -lsuif -lcheck -luseful
HEADERS =	print.h
EXPORTS =
YSRCS =
CSRCS =		doproc.c print.c
SRCS =		main.cc
OBJS =		main.o doproc.o print.o
INCLDIRFLAG =	-I$(SUIFHOME)/include
EXTRA_YFLAGS =
EXTRA_CFLAGS =	$(INCLDIRFLAG)
EXTRA_CXXFLAGS =

all:		prog
install-bin:	install-prog

include $(SUIFHOME)/Makefile.std
