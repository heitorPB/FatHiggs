# FatHiggs
Home exam for Computing Methods for High Energy Physics at Helsinki University


## Datasets generation (1)

After suffering a lot to discover how to use CMSSW, I gave up and started again
with plain Pythi8 and ROOT.

To be possible to save Pythia8 events in a root file, i had to make the
constructor of the class ResonanceWidhts (pythia8125/include/Pythia8/ResonanceWidths.h)
public. I got compilation errors when it was protected.

The Makefile downloads Pythia8 and apply the patch to make it work with root.
It also creates two binaries: signal and background. Each one produces a root file.

Smearing of the momenta is done only for photons, before the trigger.

The events are only saved if there are at least two photons with pseudorapidity < 2.5
and transverse momenta > 60 GeV/c.

The number of events that passed the trigger, trigger efficiency and the total cross
section (in mb) are saved in a ntuple "info" in the same root file as the tree with
the pythia events.


### Signal generation

To generate the signal, I am using the following Pythia settings:

* HiggsSM:all = on
* 25:m0 = 750
* 25:doForceWidth = on
* 25:mWidth = 45
* 25:onMode = off
* 25:onIfMatch = 22 22


### Background generation

To generate the background, I am using the following Pythia settings:

* PromptPhoton:ffbar2gammagamma = on
* PromptPhoton:gg2gammagamma = on
* PhaseSpace:mHatMin = 500
* PhaseSpace:mHatMax = 1000


## Data analysis (2)

talk about structure of code. Random comments.
for background, ignoring the selection.

selection for the bg: pt2 > 30 to get rid of peak around 0.

## Histogram (3)


## Questions

* 25:onIfAny or 24:onIfMatch or 25:onIfOnly?
* Why pythia is saving 2 trees in the root files?
* Why I have a nice peak of mass near zero?
* Normalization uses the number of events that passed the trigger?
* status == 1 produces no photons. (?)
* Why pythia reports higghs width = 239 ?
    - Why pythia needs doForceWidth = on when I set the mWidth?


## TODO

### 1

- [x] check if higgs width is really changed in ex 1.a
- [ ] set crosssection in ex 1
- [x] smear momenta
    - [ ] check for const references in event[i]
- [x] add trigger
- [x] limit phase space
- [x] write dataset root file
- [x] make pythia print the cross section somewhere (in root file?)
- [x] save trigger efficiency somewhere (in root file?)


### 2

- [x] get number of events passing selection
    - [x] save it somewhere


### 3

- [x] normalization
- [x] fit

### 4

- [ ] write readme explaining everything
    - [ ] explain structure of code
    - [ ] explain how to run
    - [ ] explain results
        - [ ] trigger efficiency
        - [ ] events passing selection
        - [ ] calculate number of events for luminosity of 100 / fb
        - [ ] estimate statistical significance
        - [ ] can we trust this results?
    - [ ] general comments


### general

- [ ] get rid of useless output
- [x] check photon.status == 1 somewhere
- [x] make patch for pythia to work properly
- [ ] check if units from pythia8 and root are the same:
    - [ ] 2: TLorentzVector(pythia::particle.4momentum)
    - [x] 3: in the crosssections
