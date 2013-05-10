PC=fpc
PASFLAGS=-Xs -O2
#PASFLAGS=-g

rebuild: clean all

all: enc dec lz1.ppu

%.ppu: %.pas
	$(PC) $? -fPIC $(PASFLAGS)

enc dec:
	$(PC) $@.pas

clean:
	rm -f *.o *.ppu enc dec
