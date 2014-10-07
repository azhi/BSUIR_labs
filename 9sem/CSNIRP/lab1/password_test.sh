#!/bin/bash

algs=('RC2' 'RC4' 'DES' '2k-3-DES' '3k-3-DES' 'AES-128' 'AES-192' 'AES-256')
for alg in ${algs[*]}
do
  rm -f test_out.txt test_diff.txt
  echo -ne "$(tput setaf 1)"
  wine password_crypt.exe --encode $alg 12345678 test.txt test_out.txt
  wine password_crypt.exe --decode $alg 12345678 test_out.txt test_diff.txt
  cmp test{,_diff}.txt && echo "$(tput sgr0)$(tput setaf 2)$alg is OK$(tput sgr0)"
done
