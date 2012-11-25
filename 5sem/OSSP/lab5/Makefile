ifeq ($(OS), Windows_NT)
  CC=gcc
else
  CC=i586-mingw32msvc-gcc
endif

CFLAGS=-O2 -mtune=native -std=gnu99 -D_WIN32_WINNT=0x0500
STRIP=strip
rm=rm -f

all: injector injection target strip

clean:
	$(rm) *.dll *.exe

injector:
	$(CC) $(CFLAGS) injector.c -o injector.exe

injection:
	$(CC) $(CFLAGS) -shared -Wl,-soname, injection.c -o injection.dll

target:
	$(CC) $(CFLAGS) target.c -o target.exe

strip:
	$(STRIP) injector.exe injection.dll target.exe
