# FatHiggs
Home exam for Computing Methods for High Energy Physics at Heslinki University


## Datasets generation (1)

After suffering a lot to discover how to use CMSSW, I gave up and started again
with plain Pythi8 and ROOT.

To be possible to save Pythia8 events in a root file, i had to make the
constructor of the class ResonanceWidhts (pythia8125/include/Pythia8/ResonanceWidths.h)
public. I got compilation errors when it was protected.

The Makefile downloads Pythia8 and apply the patch to make it work with root.
It also creates two binaries: signal and background. Each one produces a root file.

The events are only saved if there are at least two photons with pseudorapidity < 2.5
and transverse momenta > 60 GeV/c.

The number of events that passed the trigger, trigger efficiency and the total cross
section (in mb) are saved in a ntuple "info" in the same root file as the tree with
the pythia events.


### Signal generation

To generate the signal, I am using the following Pythia settings:

* HiggsSM:all = on
* 25:m0 = 750
* 25:mWidth = 45
* 25:onMode = off
* 25:onIfMatch = 22 22


### Background generation

To generate the background, I am using the following Pythia settings:

* PromptPhoton:ffbar2gammagamma = on
* PromptPhoton:gg2gammagamma = on
* PhaseSpace:mHatMin = 500


TODO talk about smearing, phase space. Pythia8+ROOT AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA


## Data analysis (2)

talk about structure of code. Random comments.
for background, ignoring the selection.


## Histogram (3)


## TODO

### 1

- [ ] check if higgs width is really changed in ex 1.a
- [ ] set crosssection in ex 1

- [ ] smear momenta
- [x] add trigger
- [ ] limit phase space
- [x] write dataset root file
- [x] make pythia print the cross section somewhere (in root file?)
- [x] save trigger efficiency somewhere (in root file?)


### 2

- [ ] check if units from pythia8 and root are the same, when setting TLorentzVector(pythia::particle.4momentum)
- [ ] get number of events passing selection
    - [ ] save it somewhere


### 3

- [ ] normalization
- [ ] fit

### 4

- [ ] write readme explaining everything
    - [ ] explain structure of code
    - [ ] explain how to run
    - [ ] explain results
    - [ ] general comments


### general

- [ ] get rid of useless output
- [ ] check photon.status == 1 somewhere
- [ ] make patch for pythia to work properly


## Questions

* 25:onIfAny or 24:onIfMatch or 25:onIfOnly?
* Why pythia is saveing 2 trees in the root files?
