# FatHiggs
Home exam for Computing Methods for High Energy Physics at Heslinki University

## Datasets generation

After suffering a lot to discover how to use CMSSW, I gave up and started again
with plain Pythi8.

To be possible to save Pythia8 events in a root file, i had to make the
constructor of the class ResonanceWidhts (pythia8125/include/Pythia8/ResonanceWidths.h)
public. I got compilation errors when it was protected.

### Signal generation

TODO talk about root files structure, smearing, trigger, phase space.


## TODO

### 1

[ ] check if higgs width is really changed in ex 1.a  
[ ] set crosssection in ex 1  

[ ] smear momenta  
[ ] add trigger  
[ ] limit phase space  
[x] write dataset root file  
[ ] make pythia print the cross section somewhere (in root file?)  
[ ] save trigger efficiency somewhere (in root file?)  

[ ] write readme explaining everything  
  [ ] explain structure of code  
  [ ] explain how to run  
  [ ] explain results  
  [ ] general comments  

[ ] get rid of useless output  

## Questions

* 25:onIfAny or 24:onIfMatch or 25:onIfOnly?
