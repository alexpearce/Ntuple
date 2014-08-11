#include "iostream"

#include "Ntuple.h"

Ntuple::Ntuple(const TString name) : TChain(name) {
}

Int_t Ntuple::add(const TString path) {
  Int_t ret = this->Add(path);
  this->setup_branches();
  return ret;
}

template <class T>
T Ntuple::val(const TString branch) const {
  Variant v;
  // Try find a branch called branch, else throw an exception with a message
  try {
    v = branches.at(branch);
  } catch (std::out_of_range& e) {
    std::cerr << "Branch name `" << branch << "` not in Ntuple `" << this->GetName() << "`" << std::endl;
    throw;
  }
  // Convert char to TString for simpler string comparison
  TString type_str(v.type);
  if (type_str == "D") {
    return v.D;
  } else if (type_str == "F") {
    return v.F;
  } else if (type_str == "B") {
    return v.B;
  } else if (type_str == "b") {
    return v.b;
  } else if (type_str == "S") {
    return v.S;
  } else if (type_str == "s") {
    return v.s;
  } else if (type_str == "I") {
    return v.I;
  } else if (type_str == "i") {
    return v.i;
  } else if (type_str == "L") {
    return v.L;
  } else if (type_str == "l") {
    return v.l;
  } else if (type_str == "O") {
    return v.O;
  } else {
    return static_cast<T>(0);
  }
}
// We must explictly declare the values of T that might be used
// http://stackoverflow.com/a/1639837/596068
template Double_t Ntuple::val(const TString branch) const;
template Float_t Ntuple::val(const TString branch) const;
template Char_t Ntuple::val(const TString branch) const;
template UChar_t Ntuple::val(const TString branch) const;
template Short_t Ntuple::val(const TString branch) const;
template UShort_t Ntuple::val(const TString branch) const;
template Int_t Ntuple::val(const TString branch) const;
template UInt_t Ntuple::val(const TString branch) const;
template Long64_t Ntuple::val(const TString branch) const;
template ULong64_t Ntuple::val(const TString branch) const;
template Bool_t Ntuple::val(const TString branch) const;

void Ntuple::setup_branches() {
  TObjArray *branch_list = this->GetListOfBranches();
  TBranch *branch;
  TString name;
  TString title;
  TObjArray *tokens;
  TString type_str;
  // For each branch, deduce it's primitive numerical type, then
  // create a corresponding Variant in the branches map, and assign
  // the Variant's numerical field to the branch address
  for (Int_t i = 0; i <= branch_list->GetLast(); ++i) {
    if ((branch = static_cast<TBranch *>(branch_list->At(i)))) {
      name = branch->GetName();
      title = branch->GetTitle();
      tokens = title.Tokenize("/");
      type_str = ((TObjString*)tokens->At(tokens->GetLast()))->String();
      this->branches[name].type = type_str[0];
      // TODO what shall we do if no type is found?
      if (type_str == "D") {
        this->SetBranchAddress(name, &(this->branches[name].D));
      } else if (type_str == "F") {
        this->SetBranchAddress(name, &(this->branches[name].F));
      } else if (type_str == "B") {
        this->SetBranchAddress(name, &(this->branches[name].B));
      } else if (type_str == "b") {
        this->SetBranchAddress(name, &(this->branches[name].b));
      } else if (type_str == "S") {
        this->SetBranchAddress(name, &(this->branches[name].S));
      } else if (type_str == "s") {
        this->SetBranchAddress(name, &(this->branches[name].s));
      } else if (type_str == "I") {
        this->SetBranchAddress(name, &(this->branches[name].I));
      } else if (type_str == "i") {
        this->SetBranchAddress(name, &(this->branches[name].i));
      } else if (type_str == "L") {
        this->SetBranchAddress(name, &(this->branches[name].L));
      } else if (type_str == "l") {
        this->SetBranchAddress(name, &(this->branches[name].l));
      } else if (type_str == "O") {
        this->SetBranchAddress(name, &(this->branches[name].O));
      }
    }
  }
}
