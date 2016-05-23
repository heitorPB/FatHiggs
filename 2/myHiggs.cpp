#include "myHiggs.h"

#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include <iostream>

#include "Pythia8/Pythia.h"

void MyHiggs::analyze(std::string path, TTree *out_tree)
{
	TFile in_file(path.c_str());
	TTree *genParticles;
	in_file.GetObject("gen_Particles", genParticles);

	Pythia8::Event *event = 0;
	genParticles->SetBranchAddress("events", &event);

	TLorentzVector photon1(0, 0, 0, 0);
	TLorentzVector photon2(0, 0, 0, 0);

	for (long long i = 0; i < genParticles->GetEntries(); i++) {
		genParticles->GetEntry(i);

		// TODO: get 2 highest PT
		// TODO: check condition
		//       if ok, save number of ok's, save mass in a TTree
		std::cout << "Event: " << i << "\tSize:" << event->size() << "\n";
	}

	in_file.Close();
}
