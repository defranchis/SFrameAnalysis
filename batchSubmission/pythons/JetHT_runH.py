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


  #runH
  ["JetHT_18",["JetHT_18"]],
  ["JetHT_19",["JetHT_19"]],
  ["JetHT_20",["JetHT_20"]],
  ["JetHT_21",["JetHT_21"]],
  ["JetHT_22",["JetHT_22"]],
  ["JetHT_23",["JetHT_23"]],
  ["JetHT_24",["JetHT_24"]],


  ]


userItems = [
  ["IsData","true"],
  ["IsSignal","false"],
  ["IsTTbar","false"],
  ["IsQCD","false"],
  ["IsRunH","true"],
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

