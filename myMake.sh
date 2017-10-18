
#!/bin/bash

echo "Entering NtupleVariables"
cd NtupleVariables
make
echo "Entering GoodRunsLists"
cd ../GoodRunsLists
make
echo "Entering PileupReweightingTool"
cd ../PileupReweightingTool
make
echo "Entering VHTausResonances"
cd ../VHTausResonances
make
cd ..