loadLibs=[
  #"libGenVector",
	"libNtupleVariables",
  "libGoodRunsLists",
	"libPileupReweightingTool",
  "libBTagTools",
  "libVHAllhadronicResonances"
  ]


loadPacks=[
    "SFrameCore.par",
   	"NtupleVariables.par",
    "GoodRunsLists.par",
   	"PileupReweightingTool.par",
    "BTaggingTools.par",
    "VHAllhadronicResonances.par"
	   ]

compilePacks=[
  "../NtupleVariables",
  "../GoodRunsLists",
  "../PileupReweightingTool",
  "../BTaggingTools",
  "../VHAllhadronicResonances"
  ]

AddUserItems = [
   #general settings
   ["RecoTreeName","tree"], 
  ]

#End
