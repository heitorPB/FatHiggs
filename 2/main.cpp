#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include "myHiggs.h"

#include <iostream>
#include <string>


const std::string signal_file = "../1/signal_photons.root";
const std::string background_file = "../1/background_photons.root";


int main()
{
	TFile *out_file = new TFile("myHiggs_mass.root", "RECREATE");

	TTree *signal_tree = new TTree("signal", "Signal Mass");
	TTree *background_tree = new TTree("background", "Background Mass");

	MyHiggs::analyze(signal_file, signal_tree);
	MyHiggs::analyze(background_file, background_tree);

	signal_tree->Write();
	background_tree->Write();

	delete signal_tree;
	delete background_tree;
	delete out_file;

	return 0;
}