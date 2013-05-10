#!/bin/bash

if [ -z "$1" ]
then
  fl=35
else
  fl=$1
fi

/usr/bin/env ruby <<EOR
  File.open('/tmp/a', 'wb') do |f|
   a = 1; f << "\0";
   b = 2; f << "\1\1";
   3.upto($fl) { |i| a += b; b = a - b; f << ((i-1).chr * a) }
  end
EOR

./enc /tmp/a /tmp/b
./dec /tmp/b /tmp/a.n

diff /tmp/a /tmp/a.n && echo "Files are identical"

echo -n "Delete temporary files? (Y/n) "
read res

if [ "n" != "$res" ]
then
  rm /tmp/{a,b,a.n}
fi
