loadLibs=[
  #"libGenVector",
	"libNtupleVariables",
  "libGoodRunsLists",
	"libPileupReweightingTool",
  # "libBTaggingTools",
  "libVHTausResonances"
  ]


loadPacks=[
    "SFrameCore.par",
   	"NtupleVariables.par",
    "GoodRunsLists.par",
   	"PileupReweightingTool.par",
    #"BTaggingTools.par",
    "VHTausResonances.par"
	   ]

compilePacks=[
  "../NtupleVariables",
  "../GoodRunsLists",
  "../PileupReweightingTool",
  #"../BTaggingTools",
  "../VHTausResonances"
  ]

AddUserItems = [
   #general settings
   ["RecoTreeName","tree"], 
  ]

#End
