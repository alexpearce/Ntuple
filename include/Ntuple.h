#pragma once

#include <map>

#include <TChain.h>

// Holds a single char `type` and a primitive numerical type
// The value of `type` is used to deduce the numerical type, e.g.
//     if (Variable.type == "D") {
//       Double_t val = Variant.D;
//     }
struct Variant {
  char type;
  union {
    //64 bit floating point
    Double_t D;
    // 32 bit floating point
    Float_t F;
    // 8 bit signed integer
    Char_t B;
    // 8 bit unsigned integer
    UChar_t b;
    // 16 bit signed integer
    Short_t S;
    // 16 bit unsigned integer
    UShort_t s;
    // 32 bit signed integer
    Int_t I;
    // 32 bit unsigned integer
    UInt_t i;
    // 64 bit signed integer
    Long64_t L;
    // 64 bit unsigned integer
    ULong64_t l;
    // Boolean
    Bool_t O;
  };
};

// Ntuple tries to avoid the user having to call SetBranchAddress.
// It's usage is similar to a TChain (as it inherits from it), except
// one uses Ntuple::add instead of TChain::Add.
// Retrieving a branch value of type T is quite simple
//
//     Ntuple n("TreeName");
//     n.add("/path/to/file.root");
//     // Behaves like a TChain
//     ULong64_t entries = n.GetEntries();
//     n.GetEntry(0);
//     Double_t branch_value = n.val<Double_t>("my_branch_name");
//
// `branch_value` now contains the value of `my_branch_name`.
class Ntuple : public TChain {
  public:
  // Constructor
  Ntuple(const TString name);

  // Add a TTree called this->GetName() from a TFile at path
  // Superseeds TChain::Add
  Int_t add(const TString path);

  // Retrieve the numerical value of branch, which has type T
  // It is up to the caller to know the type of branch
  template <class T>
  T val(const TString branch) const;

  private:
  // Links `branches` map values to TTree branches
  void setup_branches();

  // Maps TTree branches to Variants by branch names (TBranch::GetName)
  std::map<TString, Variant> branches;
};
