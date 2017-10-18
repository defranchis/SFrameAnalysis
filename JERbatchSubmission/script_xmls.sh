
#!/bin/bash

DIR=samples

for i in $( ls $DIR ); do

    #echo $DIR/$i
    python createXMLfile.py $DIR/$i -m 450 &

done

wait