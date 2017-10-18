#! /usr/bin/python
# -*- coding: utf-8 -*-

path2xml="$SFRAME_DIR/../batchSubmission/xmls"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
outDir="$SFRAME_DIR/../AnalysisOutput"
jobName="b2gVHJob"
cycleName="VHTausAnalysis"
nEventsMax=-1
nProcesses=1
nFiles=2
# nFiles=3
hCPU="04:30:00"
hVMEM="3000M"
postFix = ""



dataSets=[


["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_0",["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_0"]],
["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_1",["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_1"]],
["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_2",["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_2"]],
["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_3",["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_3"]],
["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_4",["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_4"]],
["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_5",["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_5"]],
["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_6",["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_6"]],
["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_7",["TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_7"]],

  ]


userItems = [
  ["IsData","false"],
  ["IsSignal","false"],
  ["IsTTbar","true"],
  ["IsTTbarUnbinned","false"],
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


