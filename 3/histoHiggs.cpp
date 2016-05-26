// based on https://root.cern.ch/root/htmldoc/guides/primer/ROOTPrimer.html
// this code is ugly. Sorry.

#include "TFile.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TNtuple.h"
//#include "TBranch.h"
#include "TF1.h"
#include "TMath.h"

#include <iostream>


// luminosity in fb^-1
const float lumi = 100.;
// limits for the histo and fit
const float xmin = 500;
const float xmax = 900;


void format_line(TAttLine* line,int col,int sty)
{
	line->SetLineWidth(5);
	line->SetLineColor(col);
	line->SetLineStyle(sty);
}


double the_gausppar(double* vars, double* pars)
{
	return pars[0]*TMath::Gaus(vars[0],pars[1],pars[2])+
	       pars[3]+pars[4]*vars[0]+pars[5]*vars[0]*vars[0];
}


int main()
{
	TCanvas canvas("plot", "Higgs", 666, 666);
	// make Canvas pretty
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(1111);
	gStyle->SetStatBorderSize(0);
	gStyle->SetStatX(.89);
	gStyle->SetStatY(.89);

	TFile my_file("../2/myHiggs_mass.root", "READ");


	// read x sec and number of events from file
	TNtuple *info_in;
	my_file.GetObject("signal_info", info_in);
	info_in->GetEntry(0);
	float *row = info_in->GetArgs();

	float xsect_sig = 6.2;
	float events_sig = row[0];
	float signal_norm = xsect_sig * lumi / events_sig;

	my_file.GetObject("background_info", info_in);
	info_in->GetEntry(0);
	row = info_in->GetArgs();
	float xsect_bg = row[3] * 1e12; // mb to fb
	float events_bg = row[0];
	float bg_norm = 4.* xsect_bg * lumi / events_bg;

	std::cout << events_sig << "\t" << xsect_sig << "\t" << signal_norm << "\n";
	std::cout << events_bg << "\t" << xsect_bg << "\t" << bg_norm << "\n";


	// make histo and fill entries (bg + sig)
	TH1D histo("histo", "Higgs; Mass (GeV); normalized cross section", 40, xmin, xmax);
	//TH1D histo_bg("histo_bg", "",                                      40, xmin, xmax);
	histo.SetMarkerStyle(8);


	TTree *signal_tree = (TTree*) my_file.Get("signal");
	TTree *bg_tree = (TTree*) my_file.Get("background");
	//signal_tree->Print();
	//bg_tree->Print();

	double m = 0;
	signal_tree->SetBranchAddress("HInvMass", &m);

	for (long long i = 0; i < signal_tree->GetEntries(); i++) {
		signal_tree->GetEntry(i);
		histo.Fill(m, signal_norm);
	}

	m = 0;
	bg_tree->SetBranchAddress("HInvMass", &m);

	for (long long i = 0; i < bg_tree->GetEntries(); i++) {
		bg_tree->GetEntry(i);
		histo.Fill(m, bg_norm);
		//histo_bg.Fill(m, bg_norm);
	}


	// fit bg only
	//histo_bg.Fit("expo");

	// fit bg + signal
	//TF1 signal("signal", "TMath::BreitWigner(x, 0, 1)", xmin, xmax);
	TF1 signal("signal", "gaus", xmin, xmax);
	TF1 bg("bg", "pol0 + expo(1)", xmin, xmax);
	TF1 data("data", "gaus + pol0(3) + expo(4)", xmin, xmax);

	data.SetParNames("Strenght", "Mean", "Sigma", "a1", "a2", "a3");
	data.SetParameters(50, 700, 100, 10, 6, -2);
	histo.Fit("data");

	//histo.DrawClone("same");



	// fit bg + data
	// plot also bg

	canvas.Print("FatHiggs.pdf");
	return 0;
}