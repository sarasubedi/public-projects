#!/bin/bash
for i in $(seq -f "%02g" 01 15);
    do
        cat tests/in$i.txt | ./driver-original.py | diff tests/out$i.txt - >/dev/null
        if [ $? -eq 0 ]; then
            echo "Test $i was successful!";
        else
            echo "Test $i failed";
        fi
done


