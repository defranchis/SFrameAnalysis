#! /usr/bin/python
# -*- coding: utf-8 -*-

path2xml="$SFRAME_DIR/../batchSubmission/xmls"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
outDir="$SFRAME_DIR/../AnalysisOutput"
jobName="b2gVHJob"
cycleName="VHTausAnalysis"
nEventsMax=-1
nProcesses=1
nFiles=3
hCPU="04:30:00"
hVMEM="3000M"
postFix = ""

dataSets=[
# ["QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0",["QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0"]],
# ["QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_1",["QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_1"]],
# ["QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_2",["QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_2"]],
# ["QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_3",["QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_3"]],

["QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0",["QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0"]],
["QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_1",["QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_1"]],
["QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_2",["QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_2"]],
["QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",["QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"]],
["QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",["QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"]],
["QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",["QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"]],

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

