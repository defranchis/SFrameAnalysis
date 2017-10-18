#! /usr/bin/python
# -*- coding: utf-8 -*-

path2xml="$SFRAME_DIR/../batchSubmission/xmls"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
outDir="$SFRAME_DIR/../AnalysisOutput"
jobName="b2gVHJob"
cycleName="VHTausAnalysis"
nEventsMax=-1
nProcesses=1
nFiles=2
# nFiles=3
hCPU="04:30:00"
hVMEM="3000M"
postFix = ""



dataSets=[


["TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",["TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"]],
["TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",["TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"]],
["TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",["TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"]],
["TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",["TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"]],



  ]


userItems = [
  ["IsData","false"],
  ["IsSignal","false"],
  ["IsTTbar","true"],
  ["IsTTbarUnbinned","false"],
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


