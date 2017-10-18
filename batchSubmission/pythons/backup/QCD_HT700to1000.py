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

["QCD_HT700to1000_0",["QCD_HT700to1000_0"]],
["QCD_HT700to1000_1",["QCD_HT700to1000_1"]],
["QCD_HT700to1000_10",["QCD_HT700to1000_10"]],
["QCD_HT700to1000_11",["QCD_HT700to1000_11"]],
["QCD_HT700to1000_12",["QCD_HT700to1000_12"]],
["QCD_HT700to1000_13",["QCD_HT700to1000_13"]],
["QCD_HT700to1000_14",["QCD_HT700to1000_14"]],
["QCD_HT700to1000_15",["QCD_HT700to1000_15"]],
["QCD_HT700to1000_2",["QCD_HT700to1000_2"]],
["QCD_HT700to1000_3",["QCD_HT700to1000_3"]],
["QCD_HT700to1000_4",["QCD_HT700to1000_4"]],
["QCD_HT700to1000_5",["QCD_HT700to1000_5"]],
["QCD_HT700to1000_6",["QCD_HT700to1000_6"]],
["QCD_HT700to1000_7",["QCD_HT700to1000_7"]],
["QCD_HT700to1000_8",["QCD_HT700to1000_8"]],
["QCD_HT700to1000_9",["QCD_HT700to1000_9"]],


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

