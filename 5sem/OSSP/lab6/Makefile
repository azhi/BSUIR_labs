#
# by Frank Rysanek <rysanek@fccps.cz>
#
# Based on the source code of ioperm.c by Marcel Telka <marcel@telka.sk>
#   - thanks a million :-)
#

# See also my_names.h
# Note that after changing the driver name, you also have
# to rename $(DRVNAME).c manually in this directory :-)
DRVNAME = drvtest

#INCLUDES = -I/usr/i686-w64-mingw32/include/ddk -I/usr/i686-w64-mingw32/include
INCLUDES =

# We could in fact just add -DMY_DRIVER_NAME=\"$(DRVNAME)\" to CFLAGS,
# but we'd have to be careful to "make clean" after changing
# the driver name here in the makefile...
#CFLAGS = -Wall $(INCLUDES) -DMY_DRIVER_NAME=\"$(DRVNAME)\"
CFLAGS = -Wall --std=c99 $(INCLUDES)

# Kernel-mode libs:
#   libntoskrnl = basic kernel-mode environment
#   libhal = WRITE_PORT_UCHAR et al.
#KRNLIBS = -lntoskrnl -lhal
KRNLIBS = -lntoskrnl

CC = gcc
DLLTOOL = dlltool
STRIP = strip

all: $(DRVNAME).sys loader.exe unreg.exe test_app.exe add.exe

# Dependencies on header files:
$(DRVNAME).sys add.exe loader.exe test_app.exe unreg.exe: my_names.h
$(DRVNAME).sys add.exe test_app.exe: my_ioctl.h
add.exe loader.exe test_app.exe unreg.exe: err_helpers.o err_helpers.h

# This shall get appended to the built-in set of suffixes supported:
.SUFFIXES: .sys .exe
# Otherwise the custom inference rules below wouldn't ever kick in.

# This is implicit, no need to define this explicitly:
#.c.o:
#	$(CC) $(CFLAGS) -c -o $@ $<

.o.exe:
	$(CC) -o $@ $< err_helpers.o

# This inference rule allows us to turn an .o into a .sys without
# much hassle, implicitly.
# The downside is, that you cannot potentially add further object files
# to get linked into the .sys driver (such as, some custom ASM routines).
# Oh wait, maybe you can... try adding your .o after the last $(CC) in the rule...
.o.sys:
	$(CC)	-Wl,--base-file,$*.base \
	-Wl,--entry,_DriverEntry@8 \
	-nostartfiles -nostdlib \
	-o junk.tmp \
	$*.o \
	$(KRNLIBS)
	-rm -f junk.tmp
	$(DLLTOOL) --dllname $*.sys \
	--base-file $*.base --output-exp $*.exp
	$(CC) -Wl,--subsystem,native \
	-Wl,--image-base,0x10000 \
	-Wl,--file-alignment,0x1000 \
	-Wl,--section-alignment,0x1000 \
	-Wl,--entry,_DriverEntry@8 \
	-Wl,$*.exp \
	-mdll -nostartfiles -nostdlib \
	-o $*.sys \
	$*.o \
	$(KRNLIBS)
	$(STRIP) $*.sys

JUNK = *.base *.exp *.o *~ junk.tmp

clean:
	rm -f $(JUNK) *.sys *.exe

semiclean:
	rm -f $(JUNK)
