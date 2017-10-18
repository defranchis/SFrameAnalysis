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

["QCD_HT300to500_0",["QCD_HT300to500_0"]],
["QCD_HT300to500_1",["QCD_HT300to500_1"]],
["QCD_HT300to500_10",["QCD_HT300to500_10"]],
["QCD_HT300to500_11",["QCD_HT300to500_11"]],
["QCD_HT300to500_12",["QCD_HT300to500_12"]],
["QCD_HT300to500_13",["QCD_HT300to500_13"]],
["QCD_HT300to500_14",["QCD_HT300to500_14"]],
["QCD_HT300to500_15",["QCD_HT300to500_15"]],
["QCD_HT300to500_16",["QCD_HT300to500_16"]],
["QCD_HT300to500_17",["QCD_HT300to500_17"]],
["QCD_HT300to500_18",["QCD_HT300to500_18"]],
["QCD_HT300to500_19",["QCD_HT300to500_19"]],
["QCD_HT300to500_2",["QCD_HT300to500_2"]],
["QCD_HT300to500_3",["QCD_HT300to500_3"]],
["QCD_HT300to500_4",["QCD_HT300to500_4"]],
["QCD_HT300to500_5",["QCD_HT300to500_5"]],
["QCD_HT300to500_6",["QCD_HT300to500_6"]],
["QCD_HT300to500_7",["QCD_HT300to500_7"]],
["QCD_HT300to500_8",["QCD_HT300to500_8"]],
["QCD_HT300to500_9",["QCD_HT300to500_9"]],


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

