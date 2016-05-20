#include "myHiggs.h"

#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include <iostream>

void MyHiggs::analyze(std::string path, TTree *out_tree)
{
	TFile in_file(path.c_str());
	TTree *genPhotons;
	in_file.GetObject("gen_Photons", genPhotons);

	int pdg_id  = 0;
	int mother1 = 0;
	int mother2 = 0;
	double px = 0;
	double pz = 0;
	double py = 0;
	double e  = 0;

	genPhotons->SetBranchAddress("pdg_id", &pdg_id);
	genPhotons->SetBranchAddress("mother1", &mother1);
	genPhotons->SetBranchAddress("mother2", &mother2);
	genPhotons->SetBranchAddress("px", &px);
	genPhotons->SetBranchAddress("py", &py);
	genPhotons->SetBranchAddress("pz", &pz);
	genPhotons->SetBranchAddress("e", &e);

	for (long long i = 0; i < genPhotons->GetEntries(); i++) {
		genPhotons->GetEntry(i);

		// TODO: get 2 highest PT
		// TODO: check condition
		//       if ok, save number of ok's, save mass in a TTree
	//	genPhotons->Fill();
	}
	//genPhotons->Write();
	//outFile->Close();
}
