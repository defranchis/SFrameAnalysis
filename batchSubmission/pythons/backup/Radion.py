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

["RadionTohhTohtatahbb_narrow_M-1000_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-1000_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-1200_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-1200_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-1400_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-1400_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-1600_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-1600_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-1800_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-1800_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-2000_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-2000_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-2500_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-2500_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-3000_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-3000_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-3500_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-3500_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-4000_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-4000_13TeV-madgraph"]],
["RadionTohhTohtatahbb_narrow_M-4500_13TeV-madgraph",["RadionTohhTohtatahbb_narrow_M-4500_13TeV-madgraph"]],

  ]

userItems = [
   ["IsData","false"],
   ["IsSignal","true"],
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

