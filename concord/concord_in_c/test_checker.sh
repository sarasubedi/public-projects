#!/bin/bash
for i in {01..15};
    do
        cat tests/in$i.txt | ./concord3 | diff tests/out$i.txt - >/dev/null
        if [ $? -eq 0 ]; then
            echo "Test $i was successful!";
        else
            echo "Test $i failed";
        fi
done

