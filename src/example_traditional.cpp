#include <iostream>
#include <vector>

#include <TROOT.h>
#include <TChain.h>
#include <TH1F.h>
#include <TH1I.h>
#include <TCanvas.h>

int main(int argc, const char *argv[]) {
  gROOT->SetBatch(kTRUE);

  TChain n("DataTree");
  n.Add("data.root");
  ULong64_t entries = n.GetEntries();
  std::cout << "Entries: " << entries << std::endl;

  Double_t gauss;
  Int_t uniform;
  n.SetBranchAddress("gauss_0", &gauss);
  n.SetBranchAddress("uniform_0", &uniform);

  TH1F gauss_h("gauss", "Gaussian distribution", 100, -5., 5.);
  TH1I uniform_h("uniform", "Uniform distribution", 101, 0, 101);
  for (ULong64_t entry = 0; entry < entries; ++entry) {
    n.GetEntry(entry);
    gauss_h.Fill(gauss);
    uniform_h.Fill(uniform);
  }

  TCanvas c("c", "c", 400, 400);
  gauss_h.Draw();
  c.SaveAs("gauss.traditional_cpp.pdf");
  c.Clear();
  uniform_h.Draw();
  c.SaveAs("uniform.traditional_cpp.pdf");

  return 0;
}
