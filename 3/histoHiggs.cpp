// based on https://root.cern.ch/root/htmldoc/guides/primer/ROOTPrimer.html

#include "TFile.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TTree.h"
#include "TCanvas.h"


const double lumi = 100.;


void format_line(TAttLine* line,int col,int sty)
{
	line->SetLineWidth(5);
	line->SetLineColor(col);
	line->SetLineStyle(sty);
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

	TTree *signal_tree;
	TTree *bg_tree;
	my_file.GetObject("signal", signal_tree);
	my_file.GetObject("background", bg_tree);
	//signal_tree->Print();
	//bg_tree->Print();

	double xsect_sig = 1.226e-16 * 1e12;
	unsigned int events_sig = 2064;
	double xsect_bg = 3.746e-10 * 1e12;
	unsigned int events_bg = 38882;

	// get data
	signal_tree->Draw("HInvMass>>histo_sig(50, 400, 1200)");
	bg_tree->Draw("HInvMass>>histo_bg(50, 400, 1200)");

	TH1F *hist_sig = (TH1F*) gDirectory->Get("histo_sig");
	hist_sig->SetMarkerStyle(8);
	hist_sig->SetNormFactor(xsect_sig * lumi / events_sig);
	hist_sig->Draw();

	TH1F *hist_bg = (TH1F*) gDirectory->Get("histo_bg");
	hist_bg->SetMarkerStyle(8);
	hist_bg->SetNormFactor(4. * xsect_bg * lumi / events_bg);
	hist_bg->Draw();

	canvas.Print("FatHiggs.pdf");
	return 0;
}