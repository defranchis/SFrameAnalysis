
#!/bin/bash

COUNTER=0

while read LINE; do

    let "COUNTER=COUNTER+1"
    echo $LINE > DY_$COUNTER.txt

    echo python createXMLfile.py samples/DY_$COUNTER.txt \&

done < aList.txt