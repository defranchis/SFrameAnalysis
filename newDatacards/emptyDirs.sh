
#!/bin/bash

dir=rootFiles

for subdir in $( ls -d $dir/* ); do
    rm $subdir/*.root
done

dir=datacards

rm $dir/*.txt

dir=txtFiles

for subdir in $( ls -d $dir/* ); do
    rm $subdir/*.txt
done

dir=outputs

for subdir in $( ls -d $dir/* ); do
    rm $subdir/*.txt
done
