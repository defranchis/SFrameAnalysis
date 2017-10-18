
#!/bin/bash

python combineCards.py bin1=datacards/radion_M1000_1.txt bin2=datacards/radion_M1000_2.txt bin3=datacards/radion_M1000_3.txt > combinedDatacards/radion_M1000.txt &
python combineCards.py bin1=datacards/radion_M1200_1.txt bin2=datacards/radion_M1200_2.txt bin3=datacards/radion_M1200_3.txt > combinedDatacards/radion_M1200.txt &
python combineCards.py bin1=datacards/radion_M1400_1.txt bin2=datacards/radion_M1400_2.txt bin3=datacards/radion_M1400_3.txt > combinedDatacards/radion_M1400.txt &
python combineCards.py bin1=datacards/radion_M1600_1.txt bin2=datacards/radion_M1600_2.txt bin3=datacards/radion_M1600_3.txt > combinedDatacards/radion_M1600.txt &
python combineCards.py bin1=datacards/radion_M1800_1.txt bin2=datacards/radion_M1800_2.txt bin3=datacards/radion_M1800_3.txt > combinedDatacards/radion_M1800.txt &
python combineCards.py bin1=datacards/radion_M2000_1.txt bin2=datacards/radion_M2000_2.txt bin3=datacards/radion_M2000_3.txt > combinedDatacards/radion_M2000.txt &
python combineCards.py bin1=datacards/radion_M2500_1.txt bin2=datacards/radion_M2500_2.txt bin3=datacards/radion_M2500_3.txt > combinedDatacards/radion_M2500.txt &
python combineCards.py bin1=datacards/radion_M3000_1.txt bin2=datacards/radion_M3000_2.txt bin3=datacards/radion_M3000_3.txt > combinedDatacards/radion_M3000.txt &
python combineCards.py bin1=datacards/radion_M3500_1.txt bin2=datacards/radion_M3500_2.txt bin3=datacards/radion_M3500_3.txt > combinedDatacards/radion_M3500.txt &
python combineCards.py bin1=datacards/radion_M4000_1.txt bin2=datacards/radion_M4000_2.txt bin3=datacards/radion_M4000_3.txt > combinedDatacards/radion_M4000.txt &
python combineCards.py bin1=datacards/radion_M4500_1.txt bin2=datacards/radion_M4500_2.txt bin3=datacards/radion_M4500_3.txt > combinedDatacards/radion_M4500.txt &

wait
