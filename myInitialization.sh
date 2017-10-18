
#!/bin/bash

cd Analysis/CMSSW_7_4_16/src
cmsenv

source ~/initializeLHAPDF.sh

cd ../../SFrameAnalysis/SFrame
source setup.sh
make

cd ..
source myMake.sh

