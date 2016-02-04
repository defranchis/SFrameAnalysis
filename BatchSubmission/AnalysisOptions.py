loadLibs=[
  #"libGenVector",
	"libD3PDVariables",
  "libGoodRunsLists",
	"libPileupReweightingTool",
  "libWHAllhadronicResonances"
  ]


loadPacks=[
    "SFrameCore.par",
   	"D3PDVariables.par",
    "GoodRunsLists.par",
   	"PileupReweightingTool.par",
    "WHAllhadronicResonances.par"
	   ]

compilePacks=[
  "../Common/D3PDVariables",
  "../Common/PileupReweightingTool",
  "../GoodRunsLists",
  "../B2G/WHAllhadronicResonances"
  ]

AddUserItems = [
   #general settings
   ["RecoTreeName","tree"], 
  ]

#End
