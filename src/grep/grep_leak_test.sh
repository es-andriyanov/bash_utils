#!/bin/bash

echo "
str          !

()
   ()  
()
	 end." > echo.txt


echo "test
CLEAN
clean
^#include" > pattern_leak.txt

tests=(
"-e regex -e ^print es_grep_lib.c -l -v -f pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c echo.txt -i -o -f pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c Makefile -c pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c echo.txt -l pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c Makefile -n pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c echo.txt -h pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c MMMMMMMM -s pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c echo.txt -o pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c Makefile -cl pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c echo.txt -ln pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c Makefile -no pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c echo.txt -hs pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c MMMMMMMM -si pattern_leak.txt"
"-e regex -e ^print es_grep_lib.c Makefile -ovi pattern_leak.txt"
)

run_test() {
    param="$var $@"
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -q --log-file=leak.log ./es_grep $param > valgrind.log
    leak=$(grep -e "LEAK SUMMARY:" -e "in use at exit:" -e "ERROR SUMMARY:" leak.log)
    echo "$leak"
}

for i in "${tests[@]}"
do
    run_test "$i"
done

rm -rf leak.log valgrind.log
