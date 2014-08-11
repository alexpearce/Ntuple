#include <iostream>
#include <vector>

#include <TROOT.h>
#include <TH1F.h>
#include <TH1I.h>
#include <TCanvas.h>

#include "Ntuple.h"

int main(int argc, const char *argv[]) {
  gROOT->SetBatch(kTRUE);

  Ntuple n("DataTree");
  n.add("data.root");
  ULong64_t entries = n.GetEntries();
  std::cout << "Entries: " << entries << std::endl;

  TH1F gauss_h("gauss", "Gaussian distribution", 100, -5., 5.);
  TH1I uniform_h("uniform", "Uniform distribution", 101, 0, 101);
  for (ULong64_t entry = 0; entry < entries; ++entry) {
    n.GetEntry(entry);
    gauss_h.Fill(n.val<Double_t>("gauss_0"));
    uniform_h.Fill(n.val<Int_t>("uniform_0"));
  }

  TCanvas c("c", "c", 400, 400);
  gauss_h.Draw();
  c.SaveAs("gauss.ntuple_cpp.pdf");
  c.Clear();
  uniform_h.Draw();
  c.SaveAs("uniform.ntuple_cpp.pdf");

  return 0;
}
