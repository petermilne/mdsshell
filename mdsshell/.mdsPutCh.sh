echo %HN=`hostname`
eval `acqcmd getNumSamples|sed -e s/ACQ32://`
echo %S0=-$pre
echo %S1=0
echo %S2=$post 
# assumes timebase definition is in tmp/t012.definitions
#cat /tmp/t012.definitions
eval `acqcmd getInternalClock | sed -e s/ACQ32://`
echo %DT=$getInternalClock 

