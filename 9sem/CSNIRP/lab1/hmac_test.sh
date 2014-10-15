wine hmac_check.exe --generate 12345678 test.txt test_hmac.bin
wine hmac_check.exe --check 12345678 test.txt test_hmac.bin
if [[ $? == "0" ]]; then
  echo "$(tput setaf 2)HMAC is OK$(tput sgr0)"
else
  echo "$(tput setaf 1)HMAC check failed$(tput sgr0)"
fi
