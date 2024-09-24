#!/bin/bash

echo "
str          !

()
   ()  
()
	 end." > echo.txt

tests=(
"es_cat_lib.h es_cat.c Makefile"
"-b -e -n -s -t -v echo.txt es_cat.c Makefile"
"-benstv echo.txt es_cat.c Makefile"
"--number-nonblank echo.txt es_cat.c"
"--number echo.txt es_cat.c"
"--squeeze-blank echo.txt es_cat.c"
"-E -T echo.txt es_cat.c"
"echo.txt es_cat.c -b"
"-e echo.txt es_cat.c -b"
"echo.txt nofile.txt -b -v"
"echo.txt es_cat.c -e"
"echo.txt es_cat.c -n"
"echo.txt es_cat.c -s"
"echo.txt es_cat.c -t"
"echo.txt es_cat.c -v"
)

run_test() {
    param="$var $@"
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -q --log-file=leak.log ./es_cat $param > valgrind.log
    leak=$(grep -e "LEAK SUMMARY:" -e "in use at exit:" -e "ERROR SUMMARY:" leak.log)
    echo "$leak"
}

for i in "${tests[@]}"
do
    run_test "$i"
done

rm -rf leak.log
