loadLibs=[
  #"libGenVector",
	"libNtupleVariables",
  "libGoodRunsLists",
	"libPileupReweightingTool",
  "libVHAllhadronicResonances"
  ]


loadPacks=[
    "SFrameCore.par",
   	"NtupleVariables.par",
    "GoodRunsLists.par",
   	"PileupReweightingTool.par",
    "VHAllhadronicResonances.par"
	   ]

compilePacks=[
  "../NtupleVariables",
  "../PileupReweightingTool",
  "../GoodRunsLists",
  "../B2G/VHAllhadronicResonances"
  ]

AddUserItems = [
   #general settings
   ["RecoTreeName","tree"], 
  ]

#End
