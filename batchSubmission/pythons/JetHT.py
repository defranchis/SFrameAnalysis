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

  ["JetHT_0",["JetHT_0"]],
  ["JetHT_1",["JetHT_1"]],
  ["JetHT_10",["JetHT_10"]],
  ["JetHT_11",["JetHT_11"]],
  ["JetHT_12",["JetHT_12"]],
  ["JetHT_13",["JetHT_13"]],
  ["JetHT_14",["JetHT_14"]],
  ["JetHT_15",["JetHT_15"]],
  ["JetHT_16",["JetHT_16"]],
  ["JetHT_17",["JetHT_17"]],
  ["JetHT_2",["JetHT_2"]],
  ["JetHT_3",["JetHT_3"]],
  ["JetHT_4",["JetHT_4"]],
  ["JetHT_5",["JetHT_5"]],
  ["JetHT_6",["JetHT_6"]],
  ["JetHT_7",["JetHT_7"]],
  ["JetHT_8",["JetHT_8"]],
  ["JetHT_9",["JetHT_9"]],

  #runH
  # ["JetHT_18",["JetHT_18"]],
  # ["JetHT_19",["JetHT_19"]],
  # ["JetHT_20",["JetHT_20"]],
  # ["JetHT_21",["JetHT_21"]],
  # ["JetHT_22",["JetHT_22"]],
  # ["JetHT_23",["JetHT_23"]],
  # ["JetHT_24",["JetHT_24"]],


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

