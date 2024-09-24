#!/bin/bash

DIFF=""
SUCCESS=0
FAIL=0

echo "
str          !

()
   ()  
()
	 end." > echo.txt

flags=(
    "b"
    "e"
    "n"
    "s"
    "t"
    "v"
)

files=(
"es_cat_lib.h"
"echo.txt Makefile"
"echo.txt es_cat.c Makefile"
)

tests=(
"es_cat_lib.h es_cat.c Makefile"
"-b -e -n -s -t -v echo.txt es_cat.c Makefile"
"-benstv echo.txt es_cat.c Makefile"
"--number-nonblank echo.txt es_cat.c"
"--number echo.txt es_cat.c"
"--squeeze-blank echo.txt es_cat.c"
"-E -T echo.txt es_cat.c"
)

run_test() {
    param="$var $@"
    "./es_cat" $param > es_cat.log
    "cat" $param > cat.log
    DIFF="$(diff -s es_cat.log cat.log)"
    if [ "$DIFF" == "Files es_cat.log and cat.log are identical" ]
    then
        let "SUCCESS++"
        echo "$FAIL|$SUCCESS success $param"
    else
        let "FAIL++"
        echo "$FAIL|$SUCCESS =FAIL=> $param"
    fi
    rm -f es_cat.log cat.log
}

for i in "${tests[@]}"
do
    run_test "$i"
done

for var1 in "${flags[@]}"
do
    for i in "${files[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${files[@]}"
            do
                var="-$var1 -$var2"
                run_test $i
                var="-$var1$var2"
                run_test $i
            done
        fi
    done
done

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${files[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    run_test $i
                    var="-$var1$var2$var3"
                    run_test $i
                done
            fi
        done
    done
done

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            for var4 in "${flags[@]}"
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${files[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        run_test "$i"
                        var="-$var1$var2$var3$var4"
                        run_test "$i"
                    done
                fi
            done
        done
    done
done

echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
