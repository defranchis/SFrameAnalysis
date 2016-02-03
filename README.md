# SFrameAnalysis

Repository using submodules for ntuple-based analysis (at PSI T3).

## Getting started

You need ROOT or CMSSW to get started, here CMSSW will be used, and the installation will be done into a new directory:
```
mkdir Analysis
cd Analysis
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_7_4_12_patch2
cd CMSSW_7_4_12_patch2/src
cmsenv
cd ../..
```
Then clone this repository (or your fork of it):
```
git clone git@github.com:clelange/SFrameAnalysis.git
cd SFrameAnalysis
source init.sh
```

Setup and compile SFrame:
```
cd SFrame
source setup.sh
make
cd ..
```

Then go into each of the other subdirectories and type ```make```
