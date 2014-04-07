action=$1
shift
count=$1
shift
filename=$1
shift

exec >$filename

while [ $count -gt 0 ]
do
    echo $action $RANDOM
    (( count-- ))
done
