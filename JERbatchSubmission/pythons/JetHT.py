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

["Run2015C_25ns-05Oct2015-v1_JetHT",["Run2015C_25ns-05Oct2015-v1_JetHT"]],

["Run2015D-PromptReco-v3_JetHT_0",["Run2015D-PromptReco-v3_JetHT_0"]],
["Run2015D-PromptReco-v3_JetHT_1",["Run2015D-PromptReco-v3_JetHT_1"]],
["Run2015D-PromptReco-v3_JetHT_2",["Run2015D-PromptReco-v3_JetHT_2"]],
["Run2015D-PromptReco-v3_JetHT_3",["Run2015D-PromptReco-v3_JetHT_3"]],

["Run2015D-PromptReco-v4_JetHT_0",["Run2015D-PromptReco-v4_JetHT_0"]],
["Run2015D-PromptReco-v4_JetHT_1",["Run2015D-PromptReco-v4_JetHT_1"]],
["Run2015D-PromptReco-v4_JetHT_2",["Run2015D-PromptReco-v4_JetHT_2"]],
["Run2015D-PromptReco-v4_JetHT_3",["Run2015D-PromptReco-v4_JetHT_3"]],
["Run2015D-PromptReco-v4_JetHT_4",["Run2015D-PromptReco-v4_JetHT_4"]],
["Run2015D-PromptReco-v4_JetHT_5",["Run2015D-PromptReco-v4_JetHT_5"]],

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

