#include "myHiggs.h"

#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TNtuple.h"

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

	// float because i will save in a TNtuple
	float events_selected = 0;

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

		// TODO this part is fucking ugly...
		// there is a weird peak near 0, we don't want it
		temp = (photon1 + photon2).M();
		if (temp < 200)
			nPhotonsSelected = 0;

		if ((1 == nPhotonsSelected) && ("signal" == tree_name)) {
			mass.push_back(temp);
			events_selected++;
			//std::cout << in_path << " M: " << temp << "\n";
		} else if (("background" == tree_name) && (temp > 200)){
			mass.push_back(temp);
			events_selected++;
		}

		/*std::cout << "Event: " << i << ""
		          << "\tnPhotons (Selected): "
		          << nPhotons << " (" << nPhotonsSelected << ")\t"
		          << "Pt1: " << photon1.Pt() << "\tPt2: "
		          << photon2.Pt() << "\n";*/
	}

	// read NTuple with info about signal/bg
	TNtuple *info_in;
	in_file.GetObject("info", info_in);
	float total_events;
	float events_passed_trigger;
	float trigger_efficiency;
	float total_cross_section;
	float *row;

	info_in->GetEntry(0);
	row = info_in->GetArgs();
	total_events = row[0];
	events_passed_trigger = row[1];
	trigger_efficiency = row[2];
	total_cross_section = row[3];

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

	std::string info_name = tree_name + "_info";
	TNtuple info_out(info_name.c_str(), info_name.c_str(),
	                 "TotalEvents:EventsPassedTrigger:TriggerEfficiency:TotalCrossSection:EventsPassedSelection");
	info_out.Fill(total_events, events_passed_trigger, trigger_efficiency, total_cross_section, events_selected);
	info_out.Write();

	std::cout << tree_name << ":\tTotalEvents: " << total_events << "\tEventsPassedTrigger: " << events_passed_trigger << " (" << trigger_efficiency * 100. << "%)\tTotalCrossSection: " << total_cross_section << "\t EventsSelected: " << events_selected << "(" << events_selected * 100. / events_passed_trigger << ")\n";

	delete out_tree;
	result_file.Close();
}
