#!/bin/bash

DIFF=""
SUCCESS=0
FAIL=0

echo "
CLEAN          !

()
   ()  
()
	 end." > echo.txt

echo "test
CLEAN
clean
^#include" > pattern.txt

flags=(
    "i"
    "v"
    "c"
    "l"
    "n"
    "h"
    "s"
    "o"
)

files=(
"-e int es_grep_lib.h"
"-f pattern.txt echo.txt Makefile"
"-e grep -f pattern.txt echo.txt es_grep.c Makefile"
)

run_test() {
    param="$var $@"
    "./es_grep" $param > es_grep.log
    "grep" $param > grep.log
    DIFF="$(diff -s es_grep.log grep.log)"
    if [ "$DIFF" == "Files es_grep.log and grep.log are identical" ]
    then
        let "SUCCESS++"
        echo "$FAIL|$SUCCESS success $param"
    else
        let "FAIL++"
        echo "$FAIL|$SUCCESS =FAIL=> $param"
    fi
    rm -f es_grep.log grep.log
}

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
