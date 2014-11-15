#!/bin/bash

pwd=$(dirname $0)
call_params_file=$pwd/call_params_easy.txt
while read line
do
  COMMAND="bundle exec ruby lab2/lab2.rb $line"
  set -x
  eval $COMMAND
  set +x
done < $call_params_file
