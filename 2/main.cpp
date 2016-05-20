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

	MyHiggs::analyze(signal_file, nullptr);
	MyHiggs::analyze(background_file, nullptr);

	delete out_file;
	return 0;
}