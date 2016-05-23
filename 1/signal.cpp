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

	outFile = new TFile("signal_photons.root", "RECREATE");

	genPhotons = new TTree("gen_Particles", "Particles generated");
	_event = 0;
	genPhotons->Branch("events", &_event);
	/*genPhotons->Branch("pdg_id", &pdg_id, "pdg_id/I");
	genPhotons->Branch("mother1", &mother1, "mother1/I");
	genPhotons->Branch("mother2", &mother2, "mother2/I");
	genPhotons->Branch("px", &px, "px/D");
	genPhotons->Branch("py", &py, "py/D");
	genPhotons->Branch("pz", &pz, "pz/D");
	genPhotons->Branch("e", &e, "e/D");*/
}


void MyAnalysis::analyze(Event& event) {
	n_events++;

	/*for (int i = 0; i < event.size(); i++) {
		if (22 == event[i].id()) {
			pdg_id = event[i].id();
			mother1 = event[i].mother1();
			mother2 = event[i].mother2();
			px = event[i].px();
			py = event[i].py();
			pz = event[i].pz();
			e = event[i].e();

			genPhotons->Fill();
		}
	}*/

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

	// create Higgs
	pythia.readString("HiggsSM:all = on");
	pythia.readString("25:m0 = 750");
	pythia.readString("25:mWidth = 45");
	pythia.readString("25:onMode = off");
	pythia.readString("25:onIfMatch = 22 22");

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
