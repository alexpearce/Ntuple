# Ntuple

The whole `TTree::SetBranchAddress` business is a pain.
In Python, it's not terribly difficult to wrap trees in a class which transparently deals with setting addresses and the like, but for a long time I couldn't find a way to mimic such a nicety in C++.

## Why bother?

Python is nice to write in, but it's slow.
(Or PyROOT is slow, but either way ROOT with Python is not quick.)
So, can we have at least some of the pleasantries in C++?
It turns out, in most ways, that we can.

The `Ntuple` class descends from `TChain`, and allows the user to get on with stuff, rather than setting branch addresses.
In Python:

    n = Ntuple("TreeName")
    n.add("/path/to/file.root")
    # Familiar TChain methods
    entries = n.GetEntries()
    n.GetEntry(0)
    value = n.val("branch_name")

Here, `value` is a numerical value: the value of `branch_name` in the zeroth tree entry.
For C++:

    Ntuple n("TreeName");
    n.add("/path/to/file.root");
    // Familiar TChain methods
    ULong64_t entries = n.GetEntries();
    n.GetEntry(0);
    Double_t value = n.val<Double_t>("branch_name");

As before, `value` now contains the value of `branch_value` in the zeroth tree entry.
Without `Ntuple`, we'd have two extra lines *per branch* we're interested in:

    Double_t branch_name;
    n.SetBranchAddress("branch_name", &branch_name);

It is this step that's handled by Ntuple, saving many lines of tedious boilerplate and allowing flexibility.

The only syntactic difference is the explicit specification of type.
This is unavoidable when working with C++, but does place some burden on the user knowing the branch type when she wants to get its value.

## Caveats

What Ntuple doesn't handle is non-primitive, non-numerical C data types such as arrays or objects.
The full list is available on the [TTree documentation](http://root.cern.ch/root/html/TTree), but are given here for completeness:

* `Char_t` — 8 bit signed int
* `Short_t` — 16 bit signed int
* `Int_t` — 32 bit signed int
* `Long64_t` — 64 bit signed int
* `Float_t` — 32 bit floating point
* `Double_T` — 64 bit floating point

and the corresponding unsigned types (e.g. `UInt_t`).

## Speed

There are two common ways of retrieving a branch value in Python.
Given a `TTree` or `TChain` instance `n`:

    n.GetEntry(0)
    value = n.branch_name
    # Or
    import numpy as np
    branch_name = np.zeros(1, dtype=dtype_of_branch)
    n.SetBranchAddress("branch_name", branch_name)
    n.GetEntry(0)
    value = branch_name[0]

The first approach is tidy and Pythonic, but very slow.
The second approach is faster, but verbose and cumbersome, closely following the traditional C++ implementation.
The user is required to know both the numerical type of the branch (when creating the `zeros` array) and to remember that `branch_name` is a 1D array.

This folder contains four examples:

1. Python with dot accessor (`tree.branch_name`)
2. Python with `Ntuple`
3. C++ with explicit `SetBranchAddress` calls
4. C++ with `Ntuple`

The (user) timings, determined with `time <example>`, for each is as follows:

1. 9.00s
2. 39.95s
3. 1.80s
4. 1.94s

The difference in timings become larger with more branches and with more entries in the `TTree`.

To run the timing tests:

    $ make time

## Requirements

Both the C++ and Python `Ntuple` class require ROOT.
The Python implementation further requires [NumPy](http://www.numpy.org/), a common Python package for scientific computing.
