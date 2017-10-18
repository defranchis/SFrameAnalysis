
#!/bin/bash

source submitList.sh
wait

cd ../AnalysisTemp/

printf "\n"
echo looking for errors...

grep -r -i 'error\|fatal' sframe_*
greprc=$?

if [[ $greprc -eq 0 ]] ; then
    printf "\n"
    echo ERROR! abort
    printf "\n"

else

    printf "\n"
    echo good! no errors found
    printf "\n"
    mv sframe_* old/
    cd ../AnalysisOutput/
    source haddScripts/run_hadd.sh
    
fi