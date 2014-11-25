require 'mkmf'

$CFLAGS += " -O2 -std=c11 -Wno-declaration-after-statement"

create_makefile('ext')
