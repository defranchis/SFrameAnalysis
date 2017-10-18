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

["QCD_HT2000toInf_0",["QCD_HT2000toInf_0"]],
["QCD_HT2000toInf_1",["QCD_HT2000toInf_1"]],


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

