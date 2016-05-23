#include "Pythia8/Pythia.h"
// ROOT
#include "TTree.h"
#include "TFile.h"


using namespace Pythia8;


class MyAnalysis {
public:
	// Constructor can be empty.
	MyAnalysis() {}

	// Initialization actions.
	void init();

	// Analysis of each new event.
	void analyze(Event& event);

	// Show final results.
	void finish();
private:
	unsigned int n_events;
	TFile *outFile;
	TTree *genPhotons;

	Event *_event;
	int pdg_id;
	int mother1;
	int mother2;
	double px;
	double py;
	double pz;
	double e;
};


void MyAnalysis::init() {
	// Initialize counter for number of events.
	n_events = 0;

	outFile = new TFile("background_photons.root", "RECREATE");

	genPhotons = new TTree("gen_Particles", "Particles generated");
	_event = 0;
	genPhotons->Branch("events", &_event);
}


void MyAnalysis::analyze(Event& event) {
	n_events++;

	_event = &event;
	genPhotons->Fill();
}


void MyAnalysis::finish() {
	genPhotons->Write();
	genPhotons->ls();
	outFile->Close();
}


int main(/*int argc, char* argv[]*/)
{
	Pythia pythia;
	pythia.readString("Main:numberOfEvents = 1000");
	pythia.readString("Main:timesAllowErrors = 100");
	// print message every n events
	pythia.readString("Next:numberCount = 1000");
	// print event information n times
	pythia.readString("Next:numberShowInfo = 1");
	// print process record n times
	pythia.readString("Next:numberShowProcess = 1");
	//print event record n times
	pythia.readString("Next:numberShowEvent = 0");
	// LHC beam
	pythia.readString("Beams:idA = 2212");
	pythia.readString("Beams:idB = 2212");
	pythia.readString("Beams:eCM = 13000.");

	// create background
	// f fbar -> gamma gamma
	pythia.readString("PromptPhoton:ffbar2gammagamma = on");
	// g g -> gamma gamma
	pythia.readString("PromptPhoton:gg2gammagamma = on");
	pythia.readString("PhaseSpace:mHatMin = 500");

	// Initialization.
	pythia.init();

	// Declare user analysis class. Do initialization part of it.
	MyAnalysis myAnalysis;
	myAnalysis.init();

	// Read in number of event and maximal number of aborts.
	int nEvent = pythia.mode("Main:numberOfEvents");
	int nAbort = pythia.mode("Main:timesAllowErrors");
	bool hasPL = pythia.flag("PartonLevel:all");

	// Begin event loop.
	int iAbort = 0;
	for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
		// Generate events. Quit if too many failures.
		if (!pythia.next()) {
			if (++iAbort < nAbort) continue;
			cout << " Event generation aborted prematurely, owing to error!\n";
			break;
		}

		// User Analysis of current event.
		myAnalysis.analyze( (hasPL ? pythia.event : pythia.process) );
	}

	// Final statistics.
	pythia.stat();

	myAnalysis.finish();

	return 0;
}
