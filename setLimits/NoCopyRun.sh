
#!/bin/bash
printf "\n"

echo combining datacards
printf "\n"
source Combine.sh
printf "\n"

echo running datacards
printf "\n"


source runDatacards.sh
wait

printf "\n"
echo compiling

source compile.sh






echo making plot

./exe_plot

printf "\n"
echo done!
printf "\n"
