
#!/bin/bash

source extSubmitAll.sh 
wait

cd ../AnalysisTemp/

grep -r 'ERROR' sframe_*
greprc=$?

if [[ $greprc -eq 0 ]] ; then
    printf "\n"
    echo ERROR! abort
    printf "\n"

else

    grep -r 'FATAL' sframe_*
    greprc=$?

    if [[ $greprc -eq 0 ]] ; then
        printf "\n"
        echo FATAL! abort
        printf "\n"

    else

        printf "\n"
        echo good! no errors found
        printf "\n"
        mv sframe_* old/
        cd ../AnalysisOutput/
        source haddScripts/executeScripts.sh
        
    fi
    
fi