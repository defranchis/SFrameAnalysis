#! /usr/bin/python
# -*- coding: utf-8 -*-

path2xml="$SFRAME_DIR/../batchSubmission/xmls"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
outDir="$SFRAME_DIR/../AnalysisOutput"
jobName="b2gVHJob"
cycleName="VHTausAnalysis"
nEventsMax=-1
nProcesses=1
nFiles=5
hCPU="04:30:00"
hVMEM="3000M"
postFix = ""



dataSets=[



["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_0",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_0"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_1",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_1"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_2",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_2"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_3",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_3"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_4",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_reHLT_ext_4"]],



  ]


userItems = [
  ["IsData","false"],
  ["IsSignal","false"],
  ["IsTTbar","true"],
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


