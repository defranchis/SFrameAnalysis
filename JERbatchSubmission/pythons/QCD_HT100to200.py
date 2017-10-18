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

["QCD_HT100to200_0",["QCD_HT100to200_0"]],
["QCD_HT100to200_1",["QCD_HT100to200_1"]],
["QCD_HT100to200_10",["QCD_HT100to200_10"]],
["QCD_HT100to200_11",["QCD_HT100to200_11"]],
["QCD_HT100to200_12",["QCD_HT100to200_12"]],
["QCD_HT100to200_2",["QCD_HT100to200_2"]],
["QCD_HT100to200_3",["QCD_HT100to200_3"]],
["QCD_HT100to200_4",["QCD_HT100to200_4"]],
["QCD_HT100to200_5",["QCD_HT100to200_5"]],
["QCD_HT100to200_6",["QCD_HT100to200_6"]],
["QCD_HT100to200_7",["QCD_HT100to200_7"]],
["QCD_HT100to200_8",["QCD_HT100to200_8"]],
["QCD_HT100to200_9",["QCD_HT100to200_9"]],


  ]


userItems = [
  ["IsData","false"],
  ["IsSignal","false"],
  ["IsTTbar","false"],
  ["IsQCD","true"],
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

