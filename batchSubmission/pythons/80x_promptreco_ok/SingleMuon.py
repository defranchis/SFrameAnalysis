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
hVMEM="1500M"
postFix = ""

dataSets=[

["SingleMuon_0",["SingleMuon_0"]],
["SingleMuon_1",["SingleMuon_1"]],
["SingleMuon_10",["SingleMuon_10"]],
["SingleMuon_11",["SingleMuon_11"]],
["SingleMuon_12",["SingleMuon_12"]],
["SingleMuon_13",["SingleMuon_13"]],
["SingleMuon_14",["SingleMuon_14"]],
["SingleMuon_15",["SingleMuon_15"]],
["SingleMuon_16",["SingleMuon_16"]],
["SingleMuon_17",["SingleMuon_17"]],
["SingleMuon_18",["SingleMuon_18"]],
["SingleMuon_19",["SingleMuon_19"]],
["SingleMuon_2",["SingleMuon_2"]],
["SingleMuon_20",["SingleMuon_20"]],
["SingleMuon_21",["SingleMuon_21"]],
["SingleMuon_22",["SingleMuon_22"]],
["SingleMuon_23",["SingleMuon_23"]],
["SingleMuon_24",["SingleMuon_24"]],
["SingleMuon_25",["SingleMuon_25"]],
["SingleMuon_26",["SingleMuon_26"]],
["SingleMuon_27",["SingleMuon_27"]],
["SingleMuon_28",["SingleMuon_28"]],
["SingleMuon_29",["SingleMuon_29"]],
["SingleMuon_3",["SingleMuon_3"]],
["SingleMuon_30",["SingleMuon_30"]],
["SingleMuon_4",["SingleMuon_4"]],
["SingleMuon_5",["SingleMuon_5"]],
["SingleMuon_6",["SingleMuon_6"]],
["SingleMuon_7",["SingleMuon_7"]],
["SingleMuon_8",["SingleMuon_8"]],
["SingleMuon_9",["SingleMuon_9"]],


  ]


userItems = [
  ["IsData","true"],
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

