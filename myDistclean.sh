
#!/bin/bash

echo "Entering NtupleVariables"
cd NtupleVariables
make distclean
echo "Entering GoodRunsLists"
cd ../GoodRunsLists
make distclean
echo "Entering BTaggingTools"
cd ../BTaggingTools
make distclean
echo "Entering PileupReweightingTool"
cd ../PileupReweightingTool
make distclean
echo "Entering VHTausResonances"
cd ../VHTausResonances
make distclean
cd ..