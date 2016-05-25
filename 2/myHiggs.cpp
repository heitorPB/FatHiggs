#include "myHiggs.h"

#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include <iostream>
#include <string>
#include <vector>

#include "Pythia8/Pythia.h"


void MyHiggs::analyze(std::string in_path, std::string out_file)
{
	// tree to read from generator level datasets
	TFile in_file(in_path.c_str(), "READ");
	TTree *genParticles;
	in_file.GetObject("gen_Particles", genParticles);

	Pythia8::Event *event = 0;
	genParticles->SetBranchAddress("events", &event);

	// temporary place to save the masses.
	// I couldn't make it save in another file while it
	// reads in_file.root
	std::vector<double> mass;
	double temp;
	// output mass in another root file
	std::string tree_name = in_path.substr(5, in_path.size() - 18);

	// loop over events
	for (long long i = 0; i < genParticles->GetEntries(); i++) {
		genParticles->GetEntry(i);

		TLorentzVector photon1(0, 0, 0, 0);
		TLorentzVector photon2(0, 0, 0, 0);
		unsigned int nPhotons = 0;
		unsigned int nPhotonsSelected = 0;

		// loop over particles. Particle 0 representes the event as a
		// whole. See Pythia docs about event record.
		for (int p = 1; p < event->size(); p++) {
			if (22 == event->at(p).id()) {
				nPhotons++;

				// get 2 highest PT photons
				if (event->at(p).pT() > photon2.Pt()) {
					photon2.SetPx(event->at(p).px());
					photon2.SetPy(event->at(p).py());
					photon2.SetPz(event->at(p).pz());
					photon2.SetE(event->at(p).e());

					if (photon2.Pt() > photon1.Pt()) {
						TLorentzVector tmp = photon2;
						photon2 = photon1;
						photon1 = tmp;
					}

					if ((photon1.Pt() > 200.) && (photon2.Pt() > 150.))
						nPhotonsSelected++;
				}
			}
		}

		// TODO: check condition
		//       if ok, save number of ok's, save mass in a TTree
		if ((1 == nPhotonsSelected) && ("signal" == tree_name)) {
			temp = (photon1 + photon2).M();
			mass.push_back(temp);
			//std::cout << in_path << " M: " << temp << "\n";
		} else if ("background" == tree_name) {
			temp = (photon1 + photon2).M();
			mass.push_back(temp);
		}

		/*std::cout << "Event: " << i << ""
		          << "\tnPhotons (Selected): "
		          << nPhotons << " (" << nPhotonsSelected << ")\t"
		          << "Pt1: " << photon1.Pt() << "\tPt2: "
		          << photon2.Pt() << "\n";*/
	}

	in_file.Close();

	TFile result_file(out_file.c_str(), "UPDATE");
	TTree *out_tree = new TTree(tree_name.c_str(), tree_name.c_str());
	double m;
	out_tree->Branch("HInvMass", &m, "HInvMass/D");
	//out_tree->GetBranch("HInvMass")->SetFile(&result_file);

	for (auto p: mass) {
		m = p;
		std::cout << tree_name << " M: " << m << "\n";
		out_tree->Fill();
	}

	out_tree->Write();
	out_tree->ls();
	delete out_tree;
	result_file.Close();
}
