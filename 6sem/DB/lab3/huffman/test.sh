if [ ! -e enc -o ! -e dec ]
then
  make
fi

if [ -z $1 ]
then
  sz=3k
else
  sz=$1
fi

dd bs=1024 count=$sz if=/dev/urandom of=/tmp/a 2>/dev/null

./enc /tmp/a /tmp/b
./dec /tmp/b /tmp/a.n

diff /tmp/a /tmp/a.n && echo "Files are identical"

echo -n "Delete temporary files? (Y/n) "
read res

if [ "n" != "$res" ]
then
  rm /tmp/{a,b,a.n}
fi
