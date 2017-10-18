
#!/bin/bash

printf "\n"
echo copying datacards
source copyDatacards.sh

printf "\n"
echo compiling
source compile.sh

echo creating script
source createCombineScript.sh

echo combining datacards
printf "\n"
source newCombine.sh
printf "\n"

echo running datacards
printf "\n"
source runDatacards.sh
wait


echo making plot
./exe_plot
printf "\n"
echo done!
printf "\n"








