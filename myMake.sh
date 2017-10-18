
#!/bin/bash

echo "Entering NtupleVariables"
cd NtupleVariables
make -j8
echo "Entering GoodRunsLists"
cd ../GoodRunsLists
make
echo "Entering PileupReweightingTool"
cd ../PileupReweightingTool
make
# echo "Entering BTaggingTools"
# cd ../BTaggingTools
# make
echo "Entering VHTausResonances"
cd ../VHTausResonances
make
cd ..