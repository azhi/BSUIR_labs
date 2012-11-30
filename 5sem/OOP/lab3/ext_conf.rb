require 'mkmf-rice'
dir_config('../lab2/grom/build')
have_library('grom', '')
create_makefile('grom_wrapper')
