#!/bin/bash

CAT_BINARY="../s21_cat" 

declare -a tests=(
    "-n"               
    "-b"               
    "-s"             
    "-e"              
    "-t"              
    "-v"              
    "-b -n"           
    "-n -s"          
    "-b -e"           
    "-t -e"           
    "-n -t -e"        
)

declare -a files=(
    "test_1_cat.txt"
    "test_2_cat.txt"
    "test_empty.txt" 
    "test_tabs.txt"   
)

for test_case in "${tests[@]}"; do
    echo "Running test: $test_case"
    for file in "${files[@]}"; do
        echo "Testing with file: $file"
        
        $CAT_BINARY $test_case $file > my_output.txt
        
        cat $test_case $file > expected_output.txt
        
        if diff -q my_output.txt expected_output.txt > /dev/null; then
            echo "Test passed for $CAT_BINARY $test_case $file"
        else
            echo "Test failed for $CAT_BINARY $test_case $file"
            echo "My output:"
            cat my_output.txt
            echo "Expected output:"
            cat expected_output.txt
        fi
        echo "-------------------"
    done
done

rm -rf my_output.txt expected_output.txt