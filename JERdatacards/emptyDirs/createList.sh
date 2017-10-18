
#!/bin/bash

for dir in $( ls -d */ ); do
    dir=${dir%/}
    echo $dir
done