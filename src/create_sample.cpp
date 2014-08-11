#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TMath.h>

// Create a TTree filled with random data, one float and one integer branch
int main(int argc, const char *argv[]) {
  TFile f("data.root", "recreate");
  TTree t("DataTree", "DataTree");

  Double_t gauss;
  Int_t uniform;
  TString gaussName = "gauss";
  TString uniformName = "uniform";
  TString gaussBranchName, uniformBranchName;
  // Create 20 branches
  for (Int_t i = 0; i < 10; ++i) {
    gaussBranchName = TString::Format("%s_%s", gaussName.Data(), TString::UItoa(i, 10).Data());
    uniformBranchName = TString::Format("%s_%s", uniformName.Data(), TString::UItoa(i, 10).Data());
    t.Branch(gaussBranchName, &gauss, gaussBranchName + "/D");
    t.Branch(uniformBranchName, &uniform, uniformBranchName + "/I");
  }

  TRandom3 rnd;
  rnd.SetSeed(0);

  // 1 million (10e6) entries
  ULong64_t entries = 1000000;
  ULong64_t entry;
  for (entry = 0; entry < entries; ++entry) {
    // Gaussian distribution of mean 0 and width 1
    gauss = rnd.Gaus(0, 1);
    // Uniform distribution between 0 and 1
    uniform = TMath::FloorNint(100.*rnd.Rndm());
    t.Fill();
  }

  f.Write();
  f.Close();

  return 0;
}
