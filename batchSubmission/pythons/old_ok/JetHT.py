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

  ["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0000_0",["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0000_0"]],
  ["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0000_1",["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0000_1"]],
  ["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0000_2",["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0000_2"]],
  ["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0001_0",["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0001_0"]],
  ["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0001_1",["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0001_1"]],
  ["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0001_2",["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0001_2"]],
  ["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0002_0",["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0002_0"]],
  ["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0002_1",["JetHT_Run2016B-PromptReco-v2_24p47fb_v2_0002_1"]],
  ["JetHT_Run2016C-PromptReco-v2_24p47fb_v2_0000_0",["JetHT_Run2016C-PromptReco-v2_24p47fb_v2_0000_0"]],
  ["JetHT_Run2016C-PromptReco-v2_24p47fb_v2_0000_1",["JetHT_Run2016C-PromptReco-v2_24p47fb_v2_0000_1"]],
  ["JetHT_Run2016C-PromptReco-v2_24p47fb_v2_0000_2",["JetHT_Run2016C-PromptReco-v2_24p47fb_v2_0000_2"]],
  ["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0000_0",["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0000_0"]],
  ["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0000_1",["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0000_1"]],
  ["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0000_2",["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0000_2"]],
  ["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0001_0",["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0001_0"]],
  ["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0001_1",["JetHT_Run2016D-PromptReco-v2_24p47fb_v2_0001_1"]],
  ["JetHT_Run2016E-PromptReco-v2_24p47fb_v2_0000_0",["JetHT_Run2016E-PromptReco-v2_24p47fb_v2_0000_0"]],
  ["JetHT_Run2016E-PromptReco-v2_24p47fb_v2_0000_1",["JetHT_Run2016E-PromptReco-v2_24p47fb_v2_0000_1"]],
  ["JetHT_Run2016E-PromptReco-v2_24p47fb_v2_0000_2",["JetHT_Run2016E-PromptReco-v2_24p47fb_v2_0000_2"]],
  ["JetHT_Run2016E-PromptReco-v2_24p47fb_v2_0001",["JetHT_Run2016E-PromptReco-v2_24p47fb_v2_0001"]],
  ["JetHT_Run2016F-PromptReco-v1_24p47fb_v2_0000_0",["JetHT_Run2016F-PromptReco-v1_24p47fb_v2_0000_0"]],
  ["JetHT_Run2016F-PromptReco-v1_24p47fb_v2_0000_1",["JetHT_Run2016F-PromptReco-v1_24p47fb_v2_0000_1"]],
  ["JetHT_Run2016F-PromptReco-v1_24p47fb_v2_0000_2",["JetHT_Run2016F-PromptReco-v1_24p47fb_v2_0000_2"]],
  ["JetHT_Run2016G-PromptReco-v1_24p47fb_27p22fb_0000_0",["JetHT_Run2016G-PromptReco-v1_24p47fb_27p22fb_0000_0"]],
  ["JetHT_Run2016G-PromptReco-v1_24p47fb_27p22fb_0000_1",["JetHT_Run2016G-PromptReco-v1_24p47fb_27p22fb_0000_1"]],
  ["JetHT_Run2016G-PromptReco-v1_24p47fb_v2_0000_0",["JetHT_Run2016G-PromptReco-v1_24p47fb_v2_0000_0"]],
  ["JetHT_Run2016G-PromptReco-v1_24p47fb_v2_0000_1",["JetHT_Run2016G-PromptReco-v1_24p47fb_v2_0000_1"]],
  ["JetHT_Run2016G-PromptReco-v1_24p47fb_v2_0000_2",["JetHT_Run2016G-PromptReco-v1_24p47fb_v2_0000_2"]],
  ["JetHT_Run2016G-PromptReco-v1_24p47fb_v2_0001",["JetHT_Run2016G-PromptReco-v1_24p47fb_v2_0001"]],

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

