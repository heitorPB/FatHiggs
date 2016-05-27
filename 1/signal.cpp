#include "Pythia8/Pythia.h"
// ROOT
#include "TTree.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TLorentzVector.h"
#include "TRandom2.h"


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
	void finish(Pythia& pythia);
private:
	unsigned int n_events;
	unsigned int passed_trigger;
	unsigned int events_passed_trigger;

	TFile *outFile;
	TTree *genPhotons;

	Event *_event;

	TRandom2 rng;
};


void MyAnalysis::init()
{
	// Initialize counter for number of events.
	n_events = 0;
	passed_trigger = 0;
	events_passed_trigger = 0;

	rng.SetSeed(0);

	outFile = new TFile("signal_photons.root", "RECREATE");

	genPhotons = new TTree("gen_Particles", "Particles generated");
	_event = 0;
	genPhotons->Branch("events", &_event);
}


void MyAnalysis::analyze(Event& event)
{
	n_events++;
	passed_trigger = 0;

	for (int i = 1; i < event.size(); i++) {
		if (22 == event[i].id()) {
			// smearing only for photons because i don't want to waste cpu
			TLorentzVector photon;
			photon.SetPx(event[i].px());
			photon.SetPy(event[i].py());
			photon.SetPz(event[i].pz());
			photon.SetE(event[i].e());

			double R = photon.Rho();
			R = rng.Gaus(R, 0.01 * R);
			double theta = photon.Theta();
			theta = rng.Gaus(theta, 2./1000);
			double phi = photon.Phi();
			phi = rng.Gaus(phi, 2./1000);

			photon.SetRho(R);
			photon.SetTheta(theta);
			photon.SetPhi(phi);

			event[i].p(photon.Px(), photon.Py(), photon.Pz(), photon.E());

			// trigger
			if ((std::abs(event[i].eta()) < 2.5) && (event[i].pT() > 60.))
				passed_trigger++;
		}
	}

	if (passed_trigger > 1) {
		events_passed_trigger++;
		_event = &event;
		genPhotons->Fill();
	}

	//std::cout << "Event: " << n_events << "\tphotons passed trigger: " << passed_trigger << "\n";
}


void MyAnalysis::finish(Pythia& pythia)
{
	TNtuple info("info", "info",
	             "TotalEvents:EventsPassedTrigger:TriggerEfficiency:TotalCrossSection");
	float total, triggered, xsec;

	total = n_events;
	triggered = events_passed_trigger;
	xsec = pythia.info.sigmaGen();

	info.Fill(total, triggered, triggered / total, xsec);
	info.Write();

	genPhotons->Write();
	//genPhotons->ls();
	outFile->Close();
}


int main(/*int argc, char* argv[]*/)
{
	Pythia pythia;
	pythia.readString("Main:numberOfEvents = 50000");
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
	pythia.readString("25:doForceWidth = on");
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

	myAnalysis.finish(pythia);

	return 0;
}
