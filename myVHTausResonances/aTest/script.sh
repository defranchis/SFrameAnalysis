
#!/bin/bash

while read line; do

    echo \#include \"/mnt/t3nfs01/data01/shome/mdefranc/myLibs/$line\"

done < ~/h.txt