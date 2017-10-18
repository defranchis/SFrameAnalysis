#! /usr/bin/python
# -*- coding: utf-8 -*-

path2xml="$SFRAME_DIR/../JERbatchSubmission/xmls"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
outDir="$SFRAME_DIR/../AnalysisOutput"
jobName="b2gVHJob"
cycleName="VHTausAnalysis"
nEventsMax=-1
nProcesses=2
nFiles=5
hCPU="00:30:00"
hVMEM="3000M"
postFix = ""

dataSets=[

["TT_TuneCUETP8M1_13TeV-powheg-pythia8_0",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_0"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_1",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_1"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_10",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_10"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_11",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_11"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_12",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_12"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_13",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_13"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_14",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_14"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_15",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_15"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_16",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_16"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_17",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_17"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_18",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_18"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_19",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_19"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_2",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_2"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_20",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_20"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_21",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_21"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_3",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_3"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_4",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_4"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_5",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_5"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_6",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_6"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_7",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_7"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_8",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_8"]],
["TT_TuneCUETP8M1_13TeV-powheg-pythia8_9",["TT_TuneCUETP8M1_13TeV-powheg-pythia8_9"]],

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

