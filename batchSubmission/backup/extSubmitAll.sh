
#!/bin/bash

python -u submitSFrame.py -j ./pythons/Radion.py --nosandbox --useEnv & 
python -u submitSFrame.py -j ./pythons/TT_ext.py --nosandbox --useEnv & 
sleep 200
python -u submitSFrame.py -j ./pythons/JetHT.py --nosandbox --useEnv & 
sleep 100
python -u submitSFrame.py -j ./pythons/QCD_HT700to1000.py --nosandbox --useEnv & 
sleep 30
python -u submitSFrame.py -j ./pythons/QCD_HT1000to1500.py --nosandbox --useEnv & 
sleep 30
python -u submitSFrame.py -j ./pythons/QCD_HT1500to2000.py --nosandbox --useEnv & 
sleep 30
python -u submitSFrame.py -j ./pythons/QCD_HT2000toInf.py --nosandbox --useEnv & 
sleep 30
python -u submitSFrame.py -j ./pythons/ST.py --nosandbox --useEnv & 
sleep 60
python -u submitSFrame.py -j ./pythons/WJetsToLNu_HT-400To600.py --nosandbox --useEnv & 
python -u submitSFrame.py -j ./pythons/WJetsToLNu_HT-600To800.py --nosandbox --useEnv & 
python -u submitSFrame.py -j ./pythons/WJetsToLNu_HT-800To1200.py --nosandbox --useEnv & 
python -u submitSFrame.py -j ./pythons/WJetsToLNu_HT-1200To2500.py --nosandbox --useEnv &
python -u submitSFrame.py -j ./pythons/WJetsToLNu_HT-2500ToInf.py --nosandbox --useEnv & 
sleep 60
python -u submitSFrame.py -j ./pythons/DYJetsToLL_M-50.py --nosandbox --useEnv & 
python -u submitSFrame.py -j ./pythons/DYJetsToLL_M-5to50.py --nosandbox --useEnv & 
python -u submitSFrame.py -j ./pythons/Diboson.py --nosandbox --useEnv & 
python -u submitSFrame.py -j ./pythons/WJetsToQQ_HT-600ToInf.py --nosandbox --useEnv & 
python -u submitSFrame.py -j ./pythons/ZJetsToQQ_HT600toInf.py --nosandbox --useEnv & 

wait