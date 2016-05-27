// based on https://root.cern.ch/root/htmldoc/guides/primer/ROOTPrimer.html
// and also http://hadron.physics.fsu.edu/~skpark/document/ROOT/RootLecture/RootLecture290305.pdf
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
const float xmin = 610;
const float xmax = 950;
const unsigned int bins = 50;


void format_line(TAttLine* line,int col,int sty)
{
	line->SetLineWidth(3);
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
	TCanvas canvas("plot", "Higgs", 1000, 1000);
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
	float events_sig = row[1]; // events that passed the trigger
	float signal_norm = xsect_sig * lumi / events_sig;

	my_file.GetObject("background_info", info_in);
	info_in->GetEntry(0);
	row = info_in->GetArgs();
	float xsect_bg = row[3] * 1e12; // mb to fb
	float events_bg = row[1];
	float bg_norm = 4.* xsect_bg * lumi / events_bg;

	std::cout << events_sig << "\t" << xsect_sig << "\t" << signal_norm << "\n";
	std::cout << events_bg << "\t" << xsect_bg << "\t" << bg_norm << "\n";


	// make histo and fill entries (bg + sig)
	TH1D histo("histo", "Higgs; M_{#gamma#gamma} (GeV); normalized cross section", bins , xmin, xmax);
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
	}


	//TF1 signal("signal", "gaus(0)", xmin, xmax);
	TF1 signal("signal", "[0] * TMath::BreitWigner(x, [1], [2])", xmin, xmax);
	format_line(&signal, kRed, 2);

	//TF1 bg("bg", "pol0(0) + expo(1)", xmin, xmax);
	TF1 bg("bg", "pol4(0)", xmin, xmax);
	format_line(&bg, kBlue, 2);

	//TF1 data("data", "gaus(0) + pol0(3) + expo(4)", xmin, xmax);
	//TF1 data("data", "[0] * TMath::BreitWigner(x, [1], [2]) + pol0(3) + expo(4)", xmin, xmax);
	TF1 data("data", "[0] * TMath::BreitWigner(x, [1], [2]) + pol4(3)", xmin, xmax);
	format_line(&data, kBlue, 1);

	data.SetParNames("Strenght", "Mean", "Sigma");
	data.SetParameters(1500, 760, 30, 2500, -50, 0, 0, 0);
	data.SetParLimits(0, 70, 3800);
	//data.SetParLimits(1, 700, 750);
	data.SetParLimits(1, 670, 800);
	data.SetParLimits(2, 1.1, 100);

	histo.Fit("data", "IM", "", xmin, xmax);

	double param[8];

	data.GetParameters(&param[0]);
	signal.SetParameters(&param[0]);
	bg.SetParameters(&param[3]);

	TH1D signal_histo(histo);
	signal_histo.Sumw2();
	signal_histo.Add(&bg, -1);

	histo.SetMinimum(-10);

	histo.Draw("e");
	signal_histo.Draw("SAME");
	signal.Draw("SAME");
	bg.Draw("SAME");

	canvas.Print("FatHiggs.pdf");
	canvas.Print("FatHiggs.png");
	return 0;
}