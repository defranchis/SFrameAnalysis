#! /usr/bin/python
# -*- coding: utf-8 -*-

path2xml="$SFRAME_DIR/../batchSubmission/xmls"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
outDir="$SFRAME_DIR/../AnalysisOutput"
jobName="b2gVHJob"
cycleName="VHTausAnalysis"
nEventsMax=-1
nProcesses=1
nFiles=10
hCPU="04:30:00"
hVMEM="1500M"
postFix = ""

dataSets=[

["WW_TuneCUETP8M1_13TeV-pythia8_0",["WW_TuneCUETP8M1_13TeV-pythia8_0"]],
["WW_TuneCUETP8M1_13TeV-pythia8_1",["WW_TuneCUETP8M1_13TeV-pythia8_1"]],
["WZ_TuneCUETP8M1_13TeV-pythia8_0",["WZ_TuneCUETP8M1_13TeV-pythia8_0"]],
["WZ_TuneCUETP8M1_13TeV-pythia8_1",["WZ_TuneCUETP8M1_13TeV-pythia8_1"]],
["WZ_TuneCUETP8M1_13TeV-pythia8_2",["WZ_TuneCUETP8M1_13TeV-pythia8_2"]],
["ZZ_TuneCUETP8M1_13TeV-pythia8_0",["ZZ_TuneCUETP8M1_13TeV-pythia8_0"]],
["ZZ_TuneCUETP8M1_13TeV-pythia8_1",["ZZ_TuneCUETP8M1_13TeV-pythia8_1"]],
["ZZ_TuneCUETP8M1_13TeV-pythia8_2",["ZZ_TuneCUETP8M1_13TeV-pythia8_2"]],

  ]


userItems = [
  ["IsData","false"],
  ["IsSignal","false"],
  ["IsTTbar","false"],
  ["IsQCD","false"],
  ]

jobOptionsFile2=open("AnalysisOptions.py", 'r')
command2=""
for i in [o for o in jobOptionsFile2.readlines()]:
  if ("#E" + "nd") in i : break
  command2+=i
jobOptionsFile2.close()
exec command2
userItems += AddUserItems

inputTrees=["ntuplizer/tree"]
outputTrees=["analysis"]

