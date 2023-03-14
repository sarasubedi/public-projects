#!/bin/bash
for i in $(seq -f "%02g" 01 15);
    do
        ./driver-new.py --in tests/in$i.txt --out outputs/_out$i.txt 
        diff tests/out$i.txt outputs/_out$i.txt >/dev/null
        if [ $? -eq 0 ]; then
            echo "Test $i was successful!";
        else
            echo "Test $i failed";
        fi
done