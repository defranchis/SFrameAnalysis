
#!/bin/bash

python -u submitSFrame.py -j ./pythons/Radion.py --nosandbox --useEnv & 
python -u submitSFrame.py -j ./pythons/TTbar.py --nosandbox --useEnv & 
sleep 30
# python -u submitSFrame.py -j ./pythons/JetHT.py --nosandbox --useEnv & 
# sleep 180
# python -u submitSFrame.py -j ./pythons/JetHT_runH.py --nosandbox --useEnv & 
# sleep 100
python -u submitSFrame.py -j ./pythons/WJets.py --nosandbox --useEnv & 
sleep 50
python -u submitSFrame.py -j ./pythons/QCD_HT.py --nosandbox --useEnv & 
sleep 30
python -u submitSFrame.py -j ./pythons/ZJets.py --nosandbox --useEnv & 

wait

# python -u submitSFrame.py -j ./pythons/SingleMuon.py --nosandbox --useEnv & 
# python -u submitSFrame.py -j ./pythons/TT_ext.py --nosandbox --useEnv & 
# python -u submitSFrame.py -j ./pythons/TTJets_noBin.py --nosandbox --useEnv & 
wait