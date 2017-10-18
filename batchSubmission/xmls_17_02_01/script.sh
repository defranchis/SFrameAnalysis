
#!/bin/bash

for file in $( ls *.xml ); do

    file=${file%".xml"}
    #echo $file
    echo [\"$file\",[\"$file\"]],
done