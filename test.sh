#!/bin/bash

try() {
    expected="$1"
    input="$2"

    ./kzcc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$expected expected, but got $actual"
        exit 1
    fi
}


try 0 '0;'
try 42 '42;'
try 21 '5+20-4;'
try 11 '10-6+7;'
try 21 '5 + 20  -  4;'
try 47 "5+6*7;"
try 15 "5*(9-6);"
try 4 "(3+5)/2;"
try 29 "1+2+3-(4+1) + 4 * (6+1);"
try 10 "-5*4 + 30;"
try 10 "+5*-4 + 30;"
try 1 "5 == 5;"
try 0 "5 == 4;"
try 1 "5 != 4;"
try 0 "4 != 4;"
try 1 "4 > 2;"
try 0 "4 > 4;"
try 1 "4 < 5;"
try 0 "4 < 2;"
try 1 "4 >= 2;"
try 1 "4 >= 4;"
try 0 "4 >= 5;"
try 1 "4 <= 5;"
try 1 "4 <= 4;"
try 0 "4 <= 2;"
try 5 "a = 5; a;"
try 10 "a=7; b = 5-2; a+b;"
try 4 "return 4;"
try 10 "return 4 + 6;"
try 15 "a = 10; return a + 5;"

echo OK
