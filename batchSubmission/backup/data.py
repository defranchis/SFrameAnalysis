path2xml="$SFRAME_DIR/../batchSubmission/xmls_15p9"
path2tmp="$SFRAME_DIR/../AnalysisTemp"
outDir="$SFRAME_DIR/../AnalysisOutput/try"
jobName="b2gVHJob"
cycleName="VHTausAnalysis"
# nEventsMax=-1
# nProcesses=2
# nFiles=5
# hCPU="00:30:00"
# hVMEM="3000M"
# postFix = ""
nEventsMax=-1
nProcesses=1
nFiles=10
hCPU="04:30:00"
hVMEM="1500M"
postFix = ""

dataSets=[
  # ["JetHT_Run2016B-PromptReco",["JetHT_Run2016B-PromptReco-v2_15p9fb_0000_0.xml"]],

  ["JetHT_Run2016B-PromptReco",[
      "JetHT_Run2016B-PromptReco-v2_15p9fb_0000_0.xml",
      "JetHT_Run2016B-PromptReco-v2_15p9fb_0000_1.xml",
      "JetHT_Run2016B-PromptReco-v2_15p9fb_0000_2.xml",
      
      ]
   ],


  ]



userItems = [
   ["IsData","true"],
   ["IsSignal","false"],
   ["IsQCD","false"],
   ["IsTTbar","false"],
  ]

jobOptionsFile2=open("AnalysisOptions.py", 'r')
command2=""
for i in [o for o in jobOptionsFile2.readlines()]:
  if ("#E" + "nd") in i : break
  command2+=i
jobOptionsFile2.close()
exec command2
userItems += AddUserItems

# inputTrees=["ntuplizer/tree"]
# outputTrees=["tree_mutau","tree_eletau" ]

inputTrees=["ntuplizer/tree"]
outputTrees=["analysis"]

# '''
# outputTrees=["analysis","tree_mutau","tree_eletau","tree_tautau" ]
# '''
