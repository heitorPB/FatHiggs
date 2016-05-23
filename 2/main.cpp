#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include "myHiggs.h"

#include <iostream>
#include <string>


const std::string signal_file = "../1/signal_photons.root";
const std::string background_file = "../1/background_photons.root";
const std::string result_file = "myHiggs_mass.root";

int main()
{
	TFile dummy(result_file.c_str(), "RECREATE");
	dummy.Close();

	MyHiggs::analyze(signal_file, result_file);

	MyHiggs::analyze(background_file, result_file);

	return 0;
}